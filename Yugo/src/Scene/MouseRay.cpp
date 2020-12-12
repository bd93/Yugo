#include "pch.h"
#include "MouseRay.h"



namespace Yugo
{
	glm::vec3 MouseRay::s_MouseRayOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 MouseRay::s_MouseRayDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 MouseRay::s_MouseEyeSpaceCoords = glm::vec3(0.0f, 0.0f, 0.0f);
	float MouseRay::s_MouseRayCollisionDistance = 0.0f;

	void MouseRay::CalculateRayOrigin(const CameraComponent& camera, float mousePosX, float mousePosY, int sceneWidth, int sceneHeight, RayInfo* info)
	{
		const auto& projection = camera.Projection;
		const auto& view = camera.View;

		// Transform mouse viewport coords to normalized device coords
		float mouseNdcX = (2.0f * mousePosX) / (float)sceneWidth - 1.0f;
		float mouseNdcY = 1.0f - (2.0f * mousePosY) / (float)sceneHeight;

		// Transform normalized device coords to clip coords
		glm::vec4 clipCoords = glm::vec4(mouseNdcX, mouseNdcY, -1.0f, 1.0f);

		// Transform clip coords to eye space coords
		glm::vec4 eyeCoords = glm::inverse(projection) * clipCoords;

		// Transform eye space coords to world coords
		glm::vec4 worldCoords = glm::inverse(view) * eyeCoords;

		s_MouseEyeSpaceCoords = glm::vec3(eyeCoords.x, eyeCoords.y, 0.0f); // Only for UI widgets purpose
		s_MouseRayOrigin = glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z);
		s_MouseRayDirection = camera.Direction;

		if (info != nullptr)
		{
			info->MouseRayOrigin = glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z);
			info->MouseRayDirection = camera.Direction;
			info->MouseEyeSpaceCoords = glm::vec3(eyeCoords.x, eyeCoords.y, 0.0f);
		}
	}

	/*
	Algorithm can be found here: 
	https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	*/
	bool MouseRay::CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin, RayInfo* info)
	{
		//glm::vec3 faceNormal = glm::vec3(0.0f, 1.0f, 0.0f);

		float denom = glm::dot(planeNormal, s_MouseRayDirection);
		if (std::abs(denom) <= 1e-4f)
			return false;
		
		float t = (distanceToCoordOrigin - glm::dot(planeNormal, s_MouseRayOrigin)) / glm::dot(planeNormal, s_MouseRayDirection);
		if (t <= 1e-4f)
			return false;

		s_MouseRayCollisionDistance = t;
		if (info != nullptr)
			info->MouseRayCollisionDistance = t;

		return true;
	}

	/*
	Algorithm can be found here: 
	https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	*/
	bool MouseRay::CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info)
	{
		const glm::mat4& model = transform.ModelMatrix;

		glm::vec3 minAABB = model * glm::vec4(mesh.MinAABB, 1.0f);
		glm::vec3 maxAABB = model * glm::vec4(mesh.MaxAABB, 1.0f);

		glm::vec3 tminVec = (minAABB - s_MouseRayOrigin) / s_MouseRayDirection;
		glm::vec3 tmaxVec = (maxAABB - s_MouseRayOrigin) / s_MouseRayDirection;

		if (tminVec.x > tmaxVec.x) std::swap(tminVec.x, tmaxVec.x);
		if (tminVec.y > tmaxVec.y) std::swap(tminVec.y, tmaxVec.y);
		if (tminVec.z > tmaxVec.z) std::swap(tminVec.z, tmaxVec.z);

		float tmin = std::max(std::max(std::min(tminVec.x, tmaxVec.x), std::min(tminVec.y, tmaxVec.y)), std::min(tminVec.z, tmaxVec.z));
		float tmax = std::min(std::min(std::max(tminVec.x, tmaxVec.x), std::max(tminVec.y, tmaxVec.y)), std::max(tminVec.z, tmaxVec.z));

		float t;
		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0)
		{
			t = tmax;
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
			t = tmax;
			return false;
		}

		t = tmin;

		s_MouseRayCollisionDistance = t;
		if (info != nullptr)
			info->MouseRayCollisionDistance = t;

		return true;
	}

	/*
	Algorithm can be found here: 
	https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	*/
	bool MouseRay::CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info)
	{
		const glm::mat4& model = transform.ModelMatrix;

		for (const auto& submesh : mesh.Submeshes)
		{
			//for (uint64_t i = 0; i < submesh.Vertices.size(); i += 3)
			for (uint64_t i = 0; i < submesh.Indices.size(); i += 3)
			{
				// Vertex ndc (Normalised Device Coordinates)
				const auto& v0ndc = submesh.Vertices[submesh.Indices[i]].Position;
				const auto& v1ndc = submesh.Vertices[submesh.Indices[i+1]].Position;
				const auto& v2ndc = submesh.Vertices[submesh.Indices[i+2]].Position;

				// Compute plane's normal
				glm::vec3 v0v1 = v1ndc - v0ndc;
				glm::vec3 v0v2 = v2ndc - v0ndc;
				glm::vec3 nNdc = glm::cross(v0v1, v0v2);

				// Plane's normal world space coordinates
				glm::vec3 n = glm::transpose(glm::inverse(model)) * glm::vec4(nNdc, 1.0f);

				// Vertex world space coordianetes
				glm::vec3 v0 = model * glm::vec4(v0ndc, 1.0f);
				glm::vec3 v1 = model * glm::vec4(v1ndc, 1.0f);
				glm::vec3 v2 = model * glm::vec4(v2ndc, 1.0f);

				/* Step 1: Finding P */

				// Check if ray and plane are parallel
				float nDotRayDirection = glm::dot(n, s_MouseRayDirection);
				if (std::abs(nDotRayDirection) < 1e-4f) // Almost 0 
					continue; // They are parallel so they don't intersect

				// Compute d parameter
				float d = glm::dot(n, v0);

				// Compute t (IMPORTANT: for some reason it needs to be "d - glm::dot" instead of "d + glm::dot"?)
				float t = (-glm::dot(n, s_MouseRayOrigin) + d) / nDotRayDirection;
				// Check if the triangle is in behind the ray
				if (t < 0) continue; // The triangle is behind 

				// Compute the intersection point
				glm::vec3 p = s_MouseRayOrigin + t * s_MouseRayDirection;

				/* Step 2: Inside-outside test */

				glm::vec3 c; // Vector perpendicular to triangle's plane 

				// edge 0
				glm::vec3 edge0 = v1 - v0;
				glm::vec3 vp0 = p - v0;
				c = glm::cross(edge0, vp0);
				if (glm::dot(n, c) < 0) continue; // P is on the right side 

				// edge 1
				glm::vec3 edge1 = v2 - v1;
				glm::vec3 vp1 = p - v1;
				c = glm::cross(edge1, vp1);
				if (glm::dot(n, c) < 0)  continue; // P is on the right side 

				// edge 2
				glm::vec3 edge2 = v0 - v2;
				glm::vec3 vp2 = p - v2;
				c = glm::cross(edge2,vp2);
				if (glm::dot(n, c) < 0) continue; // P is on the right side; 

				if (info != nullptr)
					info->MouseRayCollisionDistance = t;

				return true; // This ray hits the triangle 
			}
		}
		return false;
	}

	bool MouseRay::CheckCollisionWithSprite(const TransformComponent& transform, RayInfo* info)
	{
		float spritePosX = transform.Position.x;
		float spritePosY = transform.Position.y;

		float spriteWidth = transform.Scale.x;
		float spriteHeight = transform.Scale.y;

		float edgeY1 = spritePosY;
		float edgeY2 = spritePosY - spriteHeight;
		float edgeX1 = spritePosX;
		float edgeX2 = spritePosX + spriteWidth;

		if (s_MouseEyeSpaceCoords.x > edgeX1 && s_MouseEyeSpaceCoords.x < edgeX2 && s_MouseEyeSpaceCoords.y < edgeY1 && s_MouseEyeSpaceCoords.y > edgeY2)
			return true;
		else
			return false;
	}

	glm::vec3 MouseRay::GetIntersectionPoint()
	{
		return s_MouseRayOrigin + s_MouseRayCollisionDistance * s_MouseRayDirection;
	}

	float MouseRay::GetCollisionDistance()
	{
		return s_MouseRayCollisionDistance;
	}

}