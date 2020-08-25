#pragma once
#include "Scene/Components.h"
#include "AnimationComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

namespace Yugo
{

	/*
	This class have methods for running mesh animation;
	The main task of this class is to calculate final transformation matrices for mesh bones
	and to define methods for running, stopping and pausing animation;
	*/
	class Animation
	{
	public:
		//static void LoadAnimations(const std::string& meshFilePath, MeshComponent& mesh, AnimationComponent& animation);
		static void RunAnimation(MeshComponent& mesh, AnimationComponent& animation);
		static void PauseAnimation(AnimationComponent& animation);
		static void StopAnimation(AnimationComponent& animation);

	private:
		static glm::mat4 aiMat4ToGlmMat4(aiMatrix4x4& from);
		static glm::mat4 aiMat3ToGlmMat4(aiMatrix3x3& from);
		//// Interpolation methods for calculating bone local transformations at specific frame
		//static void CalculateInterpolatedPosition(aiVector3D& out, float animationTime, const AnimationComponent::NodeAnimation& nodeAnim);
		//static void CalculateInterpolatedRotation(aiQuaternion& out, float animationTime, const AnimationComponent::NodeAnimation& nodeAnim);
		//static void CalculateInterpolatedScaling(aiVector3D& out, float animationTime, const AnimationComponent::NodeAnimation& nodeAnim);
		//static uint32_t FindPosition(float animationTime, const AnimationComponent::NodeAnimation& nodeAnim);
		//static uint32_t FindRotation(float animationTime, const AnimationComponent::NodeAnimation& nodeAnim);
		//static uint32_t FindScaling(float animationTime, const AnimationComponent::NodeAnimation& nodeAnim);

		static void CalculateInterpolatedPosition(aiVector3D& out, float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim);
		static void CalculateInterpolatedRotation(aiQuaternion& out, float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim);
		static void CalculateInterpolatedScaling(aiVector3D& out, float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim);
		static uint32_t FindPosition(float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim);
		static uint32_t FindRotation(float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim);
		static uint32_t FindScaling(float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim);
	};

}