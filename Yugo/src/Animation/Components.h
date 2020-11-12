#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>


namespace Yugo
{

	struct AnimationComponent
	{
		std::vector<std::string> AnimationNameVec; // Names of all animations inside one file
		uint32_t NumOfAnimations = 0;
		std::string RunningAnimationName = "None";
		bool IsAnimationRunning = false;

		/*
		This struct represents data for one animation;
		*/
		struct Animation
		{

			float Duration = 0.0f;
			float TicksPerSecond = 0.0f;
			float AnimationStartingTime = 0.0f;
			float AnimationRunningTime = 0.0f;
			/*
			This struct is similar to assimp's aiNodeAnim;
			It contains keyframes data as well as node's name
			*/
			struct NodeAnimation
			{
				std::string Name;
				std::vector<aiVectorKey> PositionKeys;
				std::vector<aiQuatKey> RotationKeys;
				std::vector<aiVectorKey> ScalingKeys;
			};
			std::vector<NodeAnimation> NodeAnimationVec;
		};
		std::unordered_map<std::string, Animation> AnimationMap; // Animation name --> AnimationComponent::Animation
		
		/*
		This struct represents animation data for one vertex;
		The idea is to map bone IDs to specific vertex by how much influence (wights)
		they have on it during mesh transformation (animation);
		*/
		struct VertexBoneWeight
		{
			uint32_t BoneID[4]; // IDs of 4 bones (could be more) which have influence on this vertex
			float BoneWeight[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // Weight factors which define how much specific bone has influence on this vertex

			void AddData(uint32_t boneID, float weight)
			{
				// Fill first "empty" slot, where weight is 0
				for (uint32_t k = 0; k < 4; ++k)
				{
					if (BoneWeight[k] <= 0.0f)
					{
						BoneID[k] = boneID;
						BoneWeight[k] = weight;
						return;
					}
				}
				// It should never get here (more bones than we have space for)
				assert(0);
			}
		};
		std::map<uint32_t, VertexBoneWeight> VertexIdBoneWeightMap; // Vertex ID --> Bone Weight
		std::map<uint32_t, glm::mat4> BoneIdOffsetMatrixMap; // Bone ID --> Offset Matrix
		std::unordered_map<std::string, uint32_t> BoneNameBoneIdMap; // Bone Name --> Bone ID
		std::unordered_map<std::string, uint32_t> BoneNameNodeAnimIdMap; // Bone Name --> NodeAnim ID

		uint32_t NumOfBones = 0;

		/*
		These vectors represents tree nodes (similar to assimp's aiNode);
		When a node is visited during tree traversal, it is pushed to vector;
		*/
		std::vector<std::string> NodeNameVec;
		std::vector<std::string> NodeParentNameVec;
		std::vector<glm::mat4> NodeTransformationVec;
		std::vector<uint32_t> NumOfChildren;
	};

}