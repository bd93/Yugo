#pragma once
#include <glm/glm.hpp>

namespace Yugo
{

	/*
	Different Vertex formats;
	Struct name "Vertex****" contains info about attributes and how many of them there are,
	e.g. 1P = 1 Position, 1T = 1 Texture coords.
	*/

	struct Vertex1P
	{
		glm::vec3 Position;
	};

	struct Vertex1P1T
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
	};

	struct Vertex1P1N1T
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Vertex1P1N1T4J
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		uint32_t BoneID[4];
		float Weight[4];
	};

}