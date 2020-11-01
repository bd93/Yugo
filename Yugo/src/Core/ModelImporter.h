#pragma once
#include "Scene/Components.h"
#include "Animation/Components.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Yugo
{

	/*
	This class is model (mesh) file loader;
	It uses Assimp library for mesh and animations data loading and creates its own data structure for the purpose of
	making MeshComponent and AnimationComponent, which can be added to specific entity;
	*/
	class ModelImporter
	{
	public:
		static std::pair<sPtr<MeshComponent>, sPtr<AnimationComponent>> LoadMeshFile(const std::string& modelFilePath);

	private:
		static void ProcessNode(aiNode* node, const aiScene* assimpScene, sPtr<MeshComponent>& mesh, sPtr<AnimationComponent>& animation);
		static void CreateMesh(aiMesh* assimpMesh, const aiScene* assimpScene, std::vector<MeshComponent>& submeshes);
		static void LoadMaterial(aiMaterial* material, Material& objectMaterial);
		static void LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName, std::vector<Texture>& myMeshTextures);

		// Transform aiMatrix 4x4 or 3x3 to glm::mat 4x4 or 3x3
		static glm::mat4 aiMat4ToGlmMat4(aiMatrix4x4& from);
		static glm::mat4 aiMat3ToGlmMat4(aiMatrix3x3& from);
		
		static aiVector3D s_ModelMinAABB; // Minimum vertex of the model's AABB (Axis Aligned Bounding Box)
		static aiVector3D s_ModelMaxAABB; // Maximum vertex of the model's AABB (Axis Aligned Bounding Box)
	};

}