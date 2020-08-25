#include "pch.h"
#include "ModelImporter.h"
#include "ResourceManager.h"



namespace Yugo
{

	/*
		
		Assimp library loads model and creates the following tree structure (without animation data):
	
							 Scene
				  ---------- mRootNode
				  |	 	 --> mMeshes[] -------------------
				  |	 	 |	 mMaterials[] <-------------  |
				  |	  	 |			|					| |
				  |	 	 ---------	|					| |
	             -> Root node	  |	--->Material		| -> Mesh
					 mChildren[]  |     GetTexture(type)|	 mVertices[] (here vertices are positions)
					 m_Meshes[]	---						|	 mNormals[]
					/			(contains index			|	 mTextureCoords[]
				   /			to a mesh				|	 mFaces[] ----------> Face
				  Child node    in Scene)				---- mMaterialIndex	      mIndices[]
				  mChildren[]						(contains index
				  mMeshes[]							 to a material
				 /								     in Scene)
				/
			   ...
	

		If there is one or multiple animations inside a file, then animation data is organised like this:

						 Scene
			  ---------- mRootNode
			  |	 	 --> mMeshes[] --------------------
			  |	 	 |	 mAnimations[] ------		  |
			  |	  	 |						|		  |
			  |	 	 -------------			|		  -----------> Mesh
			  -> Root node	     |		    Animation   		   mBones[] ----> Bone
				 mName			 |			mDuration			-->mVertices[]    mName
				 mTransformation |			mTicksPerSecond		|				  mWeights[] ----
				 mParent		 |			mChannels[] --      |								|
			   --mChildren[]	 |						 |	    |								|
			   | m_Meshes[]	------				  --------		|		        VertexWeight <---
		   -----		(contains index			  |				|				mVertexId---
		   |   			to a mesh in Scene)		  |	 			|			    mWeight    |
		   ->Child node    				          |				|						   |
			 mChildren[]						  ->NodeAnim	|						   |
			 mMeshes[]								mName		----------------------------(index to a vertex in mVertices[])
			/										Positions[]
		   /										Rotations[]
		  ...										Scalings[]

	*/


	/*

		Box vertices (z axis is towards viewer):

		5 _____________6
		 /|			  /|
	   8/_|________7_/ |
		| |			|  |
		| |1________|__|2
		| /			| /
	   4|/__________|/3

	*/
	aiVector3D ModelImporter::s_ModelMinAABB; // Minimum vertex of the model's AABB (Axis Aligned Bounding Box) -> Vertex number 1
	aiVector3D ModelImporter::s_ModelMaxAABB; // Maximum vertex of the model's AABB (Axis Aligned Bounding Box) -> Vertex number 7

	/*
	Flag for setting initial s_ModelMinAABB/s_ModelMaxAABB values each time new mesh file is loaded;
	This flag is needed in order to calculate min/max AABB of the whole model, 
	which consists of one or multiple submeshes (each submesh has its own AABB);
	Algorithm first declare that model's min/max AABB is AABB of the first submesh in the tree traversal,
	then it compares if there is any submesh with lesser/greater AABB values. 
	If there is one, then declate model's min/max AABB to be AABB of that submesh.
	*/
	static bool s_SetInitialModelMinMaxAABB = true;

	static std::string s_ModelFileDirectory; // Path of the model's file directory, which is used for model's textures loading

	std::pair<sPtr<MeshComponent>, sPtr<AnimationComponent>> ModelImporter::LoadMeshFile(const std::string& modelFilePath)
	{
		sPtr<MeshComponent> mesh = sPtrCreate<MeshComponent>();
		sPtr<AnimationComponent> animation = sPtrCreate<AnimationComponent>();

		Assimp::Importer assimpImporter;

		const aiScene* assimpScene = assimpImporter.ReadFile(
			modelFilePath,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenSmoothNormals |
			aiProcess_LimitBoneWeights |
			aiProcess_GenBoundingBoxes
			//aiProcess_PopulateArmatureData
		);

		if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << assimpImporter.GetErrorString() << "\n";
			assert(0);
		}

		s_ModelFileDirectory = modelFilePath.substr(0, modelFilePath.find_last_of("\\"));

		if (assimpScene->mNumAnimations != 0) // If file contains animation data, then create animation component
		{
			mesh->HasAnimation = true;

			// Iterate over Animation Channels and map aiNodeAnim indices
			for (uint32_t i = 0; i < assimpScene->mAnimations[0]->mNumChannels; ++i)
			{
				std::string nodeName(assimpScene->mAnimations[0]->mChannels[i]->mNodeName.C_Str());
				// aiNodeAnim name = bone name, which can be equal to aiNode name, but doesnt have to (there can be more aiNodes than bones)
				animation->BoneNameNodeAnimIdMap[nodeName] = i;
			}
		}

		// Traverse tree and populate data inside mesh and animation components
		ProcessNode(assimpScene->mRootNode, assimpScene, mesh, animation);

		mesh->MinAABB = glm::vec3(s_ModelMinAABB.x, s_ModelMinAABB.y, s_ModelMinAABB.z);
		mesh->MaxAABB = glm::vec3(s_ModelMaxAABB.x, s_ModelMaxAABB.y, s_ModelMaxAABB.z);
		s_SetInitialModelMinMaxAABB = true;

		mesh->FinalTransforms.resize(animation->NumOfBones);

		// Load animation keyframe data
		animation->NumOfAnimations = assimpScene->mNumAnimations;

		for (uint32_t i = 0; i < assimpScene->mNumAnimations; ++i)
		{
			std::string animationName = std::string(assimpScene->mAnimations[i]->mName.C_Str());
			animation->AnimationNameVec.push_back(animationName);
			animation->AnimationMap[animationName] = AnimationComponent::Animation();
			animation->AnimationMap[animationName].Duration = (float)assimpScene->mAnimations[i]->mDuration;
			animation->AnimationMap[animationName].TicksPerSecond = (float)assimpScene->mAnimations[i]->mTicksPerSecond;
			
			for (uint32_t j = 0; j < assimpScene->mAnimations[i]->mNumChannels; ++j)
			{
				AnimationComponent::Animation::NodeAnimation nodeAnim;
				for (uint32_t k = 0; k < assimpScene->mAnimations[i]->mChannels[j]->mNumPositionKeys; ++k)
				{
					nodeAnim.PositionKeys.push_back(assimpScene->mAnimations[i]->mChannels[j]->mPositionKeys[k]);
				}
				for (uint32_t k = 0; k < assimpScene->mAnimations[i]->mChannels[j]->mNumRotationKeys; ++k)
				{
					nodeAnim.RotationKeys.push_back(assimpScene->mAnimations[i]->mChannels[j]->mRotationKeys[k]);
				}
				for (uint32_t k = 0; k < assimpScene->mAnimations[i]->mChannels[j]->mNumScalingKeys; ++k)
				{
					nodeAnim.ScalingKeys.push_back(assimpScene->mAnimations[i]->mChannels[j]->mScalingKeys[k]);
				}
				animation->AnimationMap[animationName].NodeAnimationVec.push_back(nodeAnim);
			}
		}

		return std::make_pair(mesh, animation);
	}

	/*
	Traverse tree of nodes (usually one node = one submesh), create submesh and then push it to submeshes vector;
	The main idea of this method is to transform assimp's tree data structure to array (vector) structures which contain ONLY necessary data;
	Model can contain one mesh or multiple meshes (submeshes)
	*/
	void ModelImporter::ProcessNode(aiNode* node, const aiScene* assimpScene, sPtr<MeshComponent>& mesh, sPtr<AnimationComponent>& animation)
	{
		animation->NodeNameVec.push_back(std::string(node->mName.C_Str()));
		if (node->mParent == NULL)
			animation->NodeParentNameVec.push_back("None");
		else
			animation->NodeParentNameVec.push_back(std::string(node->mParent->mName.C_Str()));
		animation->NodeTransformationVec.push_back(aiMat4ToGlmMat4(node->mTransformation));
		animation->NumOfChildren.push_back(node->mNumChildren);

		static uint32_t vertexID = 0;
		/*
		If node contains more than one mesh, then keep track of base vertex ID, so for the next mesh, 
		vertexID will be relative to baseVertexID, which will be equals to mNumMeshes of the previous mesh (baseVertexID = previos_mesh->mNumMeshes)
		*/
		static uint32_t baseVertexID = 0; 

		for (uint32_t i = 0; i < node->mNumMeshes; ++i) // Usually one node contains one mesh, but it can contains more than one
		{
			aiMesh* assimpMesh = assimpScene->mMeshes[node->mMeshes[i]];
			CreateMesh(assimpMesh, assimpScene, mesh->Submeshes);

			// Load animation data
			MeshComponent& submesh = mesh->Submeshes[i];
			for (uint32_t j = 0; j < assimpMesh->mNumBones; ++j)
			{
				std::string boneName(assimpMesh->mBones[j]->mName.C_Str());
				glm::mat4 offsetMatrix = aiMat4ToGlmMat4(assimpMesh->mBones[j]->mOffsetMatrix);

				animation->BoneIdOffsetMatrixMap[j] = offsetMatrix;
				animation->BoneNameBoneIdMap[boneName] = j;

				for (uint32_t k = 0; k < assimpMesh->mBones[j]->mNumWeights; ++k)
				{
					uint32_t vertexID = baseVertexID + assimpMesh->mBones[j]->mWeights[k].mVertexId;
					float boneWeight = assimpMesh->mBones[j]->mWeights[k].mWeight;

					if (animation->VertexIdBoneWeightMap.find(vertexID) != animation->VertexIdBoneWeightMap.end())
					{
						animation->VertexIdBoneWeightMap[vertexID].AddData(j, boneWeight);
					}
					else
					{
						AnimationComponent::VertexBoneWeight vertexBoneWeight;
						vertexBoneWeight.AddData(j, boneWeight);
						animation->VertexIdBoneWeightMap[vertexID] = vertexBoneWeight;
					}
				}
				++animation->NumOfBones; 
			}

			baseVertexID = node->mNumMeshes;

			for (uint32_t j = 0; j < assimpMesh->mNumVertices; ++j)
			{
				if (assimpMesh->mNumBones != 0)
				{
					for (uint32_t k = 0; k < 4; ++k)
					{
						submesh.Vertices[vertexID].BoneID[k] = animation->VertexIdBoneWeightMap[j].BoneID[k];
						submesh.Vertices[vertexID].Weight[k] = animation->VertexIdBoneWeightMap[j].BoneWeight[k];
					}
				}
				++vertexID;
			}
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
			ProcessNode(node->mChildren[i], assimpScene, mesh, animation);

		// Reset IDs for next usage
		baseVertexID = 0;
		vertexID = 0;
	}

	void ModelImporter::CreateMesh(aiMesh* assimpMesh, const aiScene* assimpScene, std::vector<MeshComponent>& submeshes)
	{
		MeshComponent mesh;

		auto& vertices = mesh.Vertices;
		auto& indices = mesh.Indices;
		auto& textures = mesh.Textures;
		auto& objMaterial = mesh.Material; // In case imported model has material

		// Minimum coordinates of the submesh AABB
		auto& minAABB = assimpMesh->mAABB.mMin;
		mesh.MinAABB = glm::vec3(minAABB.x, minAABB.y, minAABB.z);
		// Maximum coordinates of the submesh AABB
		auto& maxAABB = assimpMesh->mAABB.mMax;
		mesh.MaxAABB = glm::vec3(maxAABB.x, maxAABB.y, maxAABB.z);

		/*
		Calculate min/max AABB coordinates of the whole model (including all submeshes of the model);
		First declare that min/max AABB of the whole model is min/max AABB of the first submesh,
		then ask (during tree traversal) if other submesh has lesser/greater vertices values than currently declared min/max model AABB;
		If it has, then new min/max model AABB is min/max AABB of that submesh
		*/

		if (s_SetInitialModelMinMaxAABB)
		{
			s_ModelMinAABB = minAABB;
			s_ModelMaxAABB = maxAABB;
			s_SetInitialModelMinMaxAABB = false;
		}

		if (minAABB.x < s_ModelMinAABB.x)
			s_ModelMinAABB.x = minAABB.x;
		if (minAABB.y < s_ModelMinAABB.y)
			s_ModelMinAABB.y = minAABB.y;
		if (minAABB.z < s_ModelMinAABB.z)
			s_ModelMinAABB.z = minAABB.z;
		if (maxAABB.x > s_ModelMaxAABB.x)
			s_ModelMaxAABB.x = maxAABB.x;
		if (maxAABB.y > s_ModelMaxAABB.y)
			s_ModelMaxAABB.y = maxAABB.y;
		if (maxAABB.z > s_ModelMaxAABB.z)
			s_ModelMaxAABB.z = maxAABB.z;

		for (uint32_t i = 0; i < assimpMesh->mNumVertices; ++i)
		{
			Vertex1P1N1T4J vertex;
			glm::vec3 vector3;
			glm::vec2 vector2;

			// Assimp uses term "mVertices" for Position attribute inside one vertex
			vector3.x = assimpMesh->mVertices[i].x;
			vector3.y = assimpMesh->mVertices[i].y;
			vector3.z = assimpMesh->mVertices[i].z;
			vertex.Position = vector3;

			// [0][i] ---> "0" is first set (can contain up to 8 different coordinates), 
			vector2.x = assimpMesh->mTextureCoords[0][i].x;
			vector2.y = assimpMesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vector2;

			vector3.x = assimpMesh->mNormals[i].x;
			vector3.y = assimpMesh->mNormals[i].y;
			vector3.z = assimpMesh->mNormals[i].z;
			vertex.Normal = vector3;

			//vector3.x = assimpMesh->mTangents[i].x;
			//vector3.y = assimpMesh->mTangents[i].y;
			//vector3.z = assimpMesh->mTangents[i].z;
			//vertex.Tangent = vector3;

			//vector3.x = assimpMesh->mBitangents[i].x;
			//vector3.y = assimpMesh->mBitangents[i].y;
			//vector3.z = assimpMesh->mBitangents[i].z;
			//vertex.Bitangent = vector3;

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < assimpMesh->mNumFaces; i++)
		{
			aiFace face = assimpMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		aiMaterial* material = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

		LoadMaterial(material, objMaterial);
		LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
		LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
		LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", textures);
		LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", textures);

		submeshes.push_back(mesh);
	}

	// Create texture; Texture file is in the same folder as .obj model file; 
	void ModelImporter::LoadMaterial(aiMaterial* material, Material& objectMaterial)
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);
		float shininess;

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		objectMaterial.Diffuse = glm::vec3(color.r, color.b, color.g);
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		objectMaterial.Ambient = glm::vec3(color.r, color.b, color.g);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		objectMaterial.Specular = glm::vec3(color.r, color.b, color.g);
		material->Get(AI_MATKEY_SHININESS, shininess);
		objectMaterial.Shininess = shininess;
	}

	void ModelImporter::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName, std::vector<Texture>& myMeshTextures)
	{
		for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString aiFileName;
			material->GetTexture(type, i, &aiFileName);
			const char* cStrFileName = aiFileName.C_Str();

			std::cout << std::string(cStrFileName) << "\n";

			bool skip = false;

			if (ResourceManager::s_TextureMap.find(std::string(cStrFileName)) == ResourceManager::s_TextureMap.end())
			{
				std::string textureFilePath = s_ModelFileDirectory + "\\" + std::string(cStrFileName);
				Texture texture(textureFilePath, typeName);
				myMeshTextures.push_back(texture);
				ResourceManager::s_TextureMap[std::string(cStrFileName)] = texture;
			}
			else
			{
				myMeshTextures.push_back(ResourceManager::s_TextureMap[std::string(cStrFileName)]);
			}
		}
	}

	glm::mat4 ModelImporter::aiMat4ToGlmMat4(aiMatrix4x4& from)
	{
		glm::mat4 to;

		to[0][0] = (float)from.a1; to[0][1] = (float)from.b1;  to[0][2] = (float)from.c1; to[0][3] = (float)from.d1;
		to[1][0] = (float)from.a2; to[1][1] = (float)from.b2;  to[1][2] = (float)from.c2; to[1][3] = (float)from.d2;
		to[2][0] = (float)from.a3; to[2][1] = (float)from.b3;  to[2][2] = (float)from.c3; to[2][3] = (float)from.d3;
		to[3][0] = (float)from.a4; to[3][1] = (float)from.b4;  to[3][2] = (float)from.c4; to[3][3] = (float)from.d4;

		return to;
	}

	glm::mat4 ModelImporter::aiMat3ToGlmMat4(aiMatrix3x3& from)
	{
		glm::mat4 to;

		to[0][0] = (float)from.a1; to[0][1] = (float)from.b1;  to[0][2] = (float)from.c1; to[0][3] = 0.0f;
		to[1][0] = (float)from.a2; to[1][1] = (float)from.b2;  to[1][2] = (float)from.c2; to[1][3] = 0.0f;
		to[2][0] = (float)from.a3; to[2][1] = (float)from.b3;  to[2][2] = (float)from.c3; to[2][3] = 0.0f;
		to[3][0] =			 0.0f; to[3][1] =			0.0f;  to[3][2] =			0.0f; to[3][3] = 1.0f;

		return to;
	}
}