#include "pch.h"
#include "Animation.h"
#include "Core/Time.h"


namespace Yugo
{

	/*

	Assimp library loads model and creates the following tree structure (only animation data is shown):

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
	
		
	Algorithm can be found here: http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
	
	*/

	/*
	This node will be pushed on the stack;
	Node's parent transformation will be pushed on the stack in order to simulate transformation passing through tree traversal;
	In order to create FinalTransforms vector, first we need to iterate over all nodes and multiply node's parent transformation matrix with node's transformation matrix;
	Second thing to do is to multiply the result of mentioned multiplication with bone offset matrix;
	*/
	struct StackNode
	{
		std::string Name;
		glm::mat4 CurrentTransform;

		StackNode(const std::string& name, const glm::mat4& currentTransform)
		{
			Name = name;
			CurrentTransform = currentTransform;
		}
		StackNode() = default;
		StackNode(const StackNode& other) = default;
	};
	static std::stack<StackNode> s_NodeStack;


	void Animation::RunAnimation(MeshComponent& mesh, AnimationComponent& animation)
	{
		const auto& runningAnimationName = animation.RunningAnimationName;
		auto& runningAnimation = animation.AnimationMap[runningAnimationName];

		runningAnimation.AnimationRunningTime = (Time::CurrentRealTime() - runningAnimation.AnimationStartingTime);
		//runningAnimation.AnimationRunningTime = (Time::CurrentRealTime() - runningAnimation.AnimationStartingTime) * 1000.0f;

		float ticksPerSecond = runningAnimation.TicksPerSecond != 0 ? runningAnimation.TicksPerSecond : 25.0f;
		float totalTicks = runningAnimation.AnimationRunningTime * ticksPerSecond;
		float animationTime = std::fmod(totalTicks, runningAnimation.Duration);

		for (uint32_t i = 0; i < animation.NodeTransformationVec.size(); ++i)
		{
			std::string nodeName = animation.NodeNameVec[i];
			std::string nodeParentName = animation.NodeParentNameVec[i];
			glm::mat4 nodeTransform = animation.NodeTransformationVec[i];

			// If bone name is the same as node name
			if (animation.BoneNameBoneIdMap.find(nodeName) != animation.BoneNameBoneIdMap.end())
			{
				// If bone has its animation node defined, then calculate node transform by interpolating data between two keyframes
				if (animation.BoneNameNodeAnimIdMap.find(nodeName) != animation.BoneNameNodeAnimIdMap.end())
				{
					const auto& nodeAnim = runningAnimation.NodeAnimationVec[animation.BoneNameNodeAnimIdMap[nodeName]];

					// Translation
					aiVector3D positionVec;
					CalculateInterpolatedPosition(positionVec, animationTime, nodeAnim);
					aiMatrix4x4 aiTranslateMatrix;
					aiMatrix4x4::Translation(positionVec, aiTranslateMatrix);
					glm::mat4 translateMatrix = aiMat4ToGlmMat4(aiTranslateMatrix);

					// Rotation
					aiQuaternion rotationQuat;
					CalculateInterpolatedRotation(rotationQuat, animationTime, nodeAnim);
					glm::mat4 rotationMatrix = aiMat3ToGlmMat4(rotationQuat.GetMatrix());

					// Scaling
					aiVector3D scalingVec;
					CalculateInterpolatedScaling(scalingVec, animationTime, nodeAnim);
					aiMatrix4x4 aiScalingMatrix;
					aiMatrix4x4::Scaling(scalingVec, aiScalingMatrix);
					glm::mat4 scalingMatrix = aiMat4ToGlmMat4(aiScalingMatrix);

					nodeTransform = translateMatrix * rotationMatrix * scalingMatrix;
				}

				if (nodeParentName == "None") // Root node
				{
					glm::mat4 currentTransform = nodeTransform;
					s_NodeStack.emplace(nodeName, currentTransform);

					auto boneID = animation.BoneNameBoneIdMap[nodeName];
					mesh.FinalTransforms[boneID] = currentTransform * animation.BoneIdOffsetMatrixMap[boneID];

					continue;
				}

				if (animation.NumOfChildren[i] > 0 && nodeParentName == s_NodeStack.top().Name)
				{
					glm::mat4 currentTransform = s_NodeStack.top().CurrentTransform * nodeTransform;
					s_NodeStack.emplace(nodeName, currentTransform);

					auto boneID = animation.BoneNameBoneIdMap[nodeName];
					mesh.FinalTransforms[boneID] = currentTransform * animation.BoneIdOffsetMatrixMap[boneID];

					continue;
				}

				if (animation.NumOfChildren[i] == 0 && nodeParentName == s_NodeStack.top().Name)
				{
					glm::mat4 currentTransform = s_NodeStack.top().CurrentTransform * nodeTransform;

					auto boneID = animation.BoneNameBoneIdMap[nodeName];
					mesh.FinalTransforms[boneID] = currentTransform * animation.BoneIdOffsetMatrixMap[boneID];

					continue;
				}

				if (animation.NumOfChildren[i] == 0 && nodeParentName != s_NodeStack.top().Name)
				{
					while (s_NodeStack.top().Name != nodeParentName)
						s_NodeStack.pop();

					continue;
				}
				if (animation.NumOfChildren[i] > 0 && nodeParentName != s_NodeStack.top().Name)
				{
					while (s_NodeStack.top().Name != nodeParentName)
						s_NodeStack.pop();

					glm::mat4 currentTransform = s_NodeStack.top().CurrentTransform * nodeTransform;
					s_NodeStack.emplace(nodeName, currentTransform);

					auto boneID = animation.BoneNameBoneIdMap[nodeName];
					mesh.FinalTransforms[boneID] = currentTransform * animation.BoneIdOffsetMatrixMap[boneID];

					continue;
				}
			}
			else
			{
				if (nodeParentName == "None") // Root node
				{
					glm::mat4 currentTransform = nodeTransform;
					s_NodeStack.emplace(nodeName, currentTransform);

					continue;
				}

				if (animation.NumOfChildren[i] > 0 && nodeParentName == s_NodeStack.top().Name)
				{
					glm::mat4 currentTransform = s_NodeStack.top().CurrentTransform * nodeTransform;
					s_NodeStack.emplace(nodeName, currentTransform);

					continue;
				}

				if (animation.NumOfChildren[i] == 0 && nodeParentName != s_NodeStack.top().Name)
				{
					while (s_NodeStack.top().Name != nodeParentName)
						s_NodeStack.pop();

					continue;
				}

				if (animation.NumOfChildren[i] > 0 && nodeParentName != s_NodeStack.top().Name)
				{
					while (s_NodeStack.top().Name != nodeParentName)
						s_NodeStack.pop();

					glm::mat4 currentTransform = s_NodeStack.top().CurrentTransform * nodeTransform;
					s_NodeStack.emplace(nodeName, currentTransform);

					continue;
				}
			}
		}

		// Empty stack for next usage
		while (!s_NodeStack.empty())
			s_NodeStack.pop();
	}

	void Animation::PauseAnimation(AnimationComponent& animation)
	{
	}

	void Animation::StopAnimation(AnimationComponent& animation)
	{
		animation.IsAnimationRunning = false;
	}

	glm::mat4 Animation::aiMat4ToGlmMat4(aiMatrix4x4& from)
	{
		glm::mat4 to;

		to[0][0] = (float)from.a1; to[0][1] = (float)from.b1;  to[0][2] = (float)from.c1; to[0][3] = (float)from.d1;
		to[1][0] = (float)from.a2; to[1][1] = (float)from.b2;  to[1][2] = (float)from.c2; to[1][3] = (float)from.d2;
		to[2][0] = (float)from.a3; to[2][1] = (float)from.b3;  to[2][2] = (float)from.c3; to[2][3] = (float)from.d3;
		to[3][0] = (float)from.a4; to[3][1] = (float)from.b4;  to[3][2] = (float)from.c4; to[3][3] = (float)from.d4;

		return to;
	}

	glm::mat4 Animation::aiMat3ToGlmMat4(aiMatrix3x3& from)
	{
		glm::mat4 to;

		to[0][0] = (float)from.a1; to[0][1] = (float)from.b1;  to[0][2] = (float)from.c1; to[0][3] = 0.0f;
		to[1][0] = (float)from.a2; to[1][1] = (float)from.b2;  to[1][2] = (float)from.c2; to[1][3] = 0.0f;
		to[2][0] = (float)from.a3; to[2][1] = (float)from.b3;  to[2][2] = (float)from.c3; to[2][3] = 0.0f;
		to[3][0] =			 0.0f; to[3][1] =			0.0f;  to[3][2] =			0.0f; to[3][3] = 1.0f;

		return to;
	}

	void Animation::CalculateInterpolatedPosition(aiVector3D& out, float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim)
	{
		// We need at least two values to interpolate
		if (nodeAnim.PositionKeys.size() == 1)
		{
			out = nodeAnim.PositionKeys[0].mValue;
			return;
		}

		unsigned int positionIndex = FindPosition(animationTime, nodeAnim);
		unsigned int nextPositionIndex = (positionIndex + 1);
		assert(nextPositionIndex < nodeAnim.PositionKeys.size());
		float deltaTime = nodeAnim.PositionKeys[nextPositionIndex].mTime - nodeAnim.PositionKeys[positionIndex].mTime;
		float factor = (animationTime - nodeAnim.PositionKeys[positionIndex].mTime) / deltaTime;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = nodeAnim.PositionKeys[positionIndex].mValue;
		aiVector3D end = nodeAnim.PositionKeys[nextPositionIndex].mValue;
		aiVector3D delta = end - start;
		out = start + factor * delta;
	}

	void Animation::CalculateInterpolatedRotation(aiQuaternion& out, float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim)
	{
		// We need at least two values to interpolate
		if (nodeAnim.RotationKeys.size() == 1)
		{
			out = nodeAnim.RotationKeys[0].mValue;
			return;
		}

		unsigned int rotationIndex = FindRotation(animationTime, nodeAnim);
		unsigned int nextRotationIndex = (rotationIndex + 1);
		assert(nextRotationIndex < nodeAnim.RotationKeys.size());
		float deltaTime = nodeAnim.RotationKeys[nextRotationIndex].mTime - nodeAnim.RotationKeys[rotationIndex].mTime;
		float factor = (animationTime - nodeAnim.RotationKeys[rotationIndex].mTime) / deltaTime;
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiQuaternion& startRotationQ = nodeAnim.RotationKeys[rotationIndex].mValue;
		const aiQuaternion& endRotationQ = nodeAnim.RotationKeys[nextRotationIndex].mValue;
		aiQuaternion::Interpolate(out, startRotationQ, endRotationQ, factor);
		out = out.Normalize();
	}

	void Animation::CalculateInterpolatedScaling(aiVector3D& out, float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim)
	{
		// We need at least two values to interpolate...
		if (nodeAnim.ScalingKeys.size() == 1)
		{
			out = nodeAnim.ScalingKeys[0].mValue;
			return;
		}

		unsigned int scaleIndex = FindScaling(animationTime, nodeAnim);
		unsigned int nextScaleIndex = (scaleIndex + 1);
		assert(nextScaleIndex < nodeAnim.ScalingKeys.size());
		float deltaTime = nodeAnim.ScalingKeys[nextScaleIndex].mTime - nodeAnim.ScalingKeys[scaleIndex].mTime;
		float factor = (animationTime - nodeAnim.ScalingKeys[scaleIndex].mTime) / deltaTime;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = nodeAnim.ScalingKeys[scaleIndex].mValue;
		aiVector3D end = nodeAnim.ScalingKeys[nextScaleIndex].mValue;
		aiVector3D delta = end - start;
		out = start + factor * delta;
	}

	uint32_t Animation::FindPosition(float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim)
	{
		assert(nodeAnim.PositionKeys.size() > 0);

		for (uint32_t i = 0; i < nodeAnim.PositionKeys.size() - 1; i++) {
			if (animationTime < nodeAnim.PositionKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);
	}

	uint32_t Animation::FindRotation(float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim)
	{
		assert(nodeAnim.RotationKeys.size() > 0);

		for (uint32_t i = 0; i < nodeAnim.RotationKeys.size() - 1; i++) {
			if (animationTime < nodeAnim.RotationKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);
	}

	uint32_t Animation::FindScaling(float animationTime, const AnimationComponent::Animation::NodeAnimation& nodeAnim)
	{
		assert(nodeAnim.ScalingKeys.size() > 0);

		for (uint32_t i = 0; i < nodeAnim.ScalingKeys.size() - 1; i++) {
			if (animationTime < nodeAnim.ScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);
	}

}