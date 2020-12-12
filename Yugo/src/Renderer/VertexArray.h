#pragma once
#include "Buffer.h"
#include "Vertex.h"
#include "Core/Core.h"


namespace Yugo
{

	template<typename VertexType>
	class VertexArray
	{
	private:
		uint32_t m_VertexArrayId;
		struct VertexAttribute
		{
			std::string Name;
			uint8_t NumOfComponents; // Number of components per vertex attribute (e.g. 3 floats for position attribute)
			size_t Offset; // Offset of the first component of the first vertex attribute in array (e.g. offset of struct's members)
		};
		std::vector<VertexAttribute> m_VertexAttributes;

		void SetVertexAttributes();

	public:
		VertexArray();
		~VertexArray();

		void SetLayout(); // Set initial layout for specific Vertex type
		void AddLayout(int index, int NumOfComponents, size_t stride, size_t offset); // Add additional layout (e.g. from another buffer)
		void SetDivisor(int vertexAttributeIndex, int divisor); // Set divisor, the number of instances that will pass between attribute update
		void Bind();
		void Unbind();
	};


	template<typename VertexType>
	inline VertexArray<VertexType>::VertexArray()
	{
		glGenVertexArrays(1, &m_VertexArrayId);
	}

	template<typename VertexType>
	inline VertexArray<VertexType>::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	template<>
	inline void VertexArray<Vertex1P>::SetVertexAttributes()
	{
		m_VertexAttributes = { 
			{"Position", 3, offsetof(Vertex1P, Position)} 
		};
	}

	template<>
	inline void VertexArray<Vertex1P1T>::SetVertexAttributes()
	{
		m_VertexAttributes = { 
			{"Position", 3, offsetof(Vertex1P1T, Position)}, 
			{"TexCoords", 2, offsetof(Vertex1P1T, TexCoords)} 
		};
	}

	template<>
	inline void VertexArray<Vertex1P1N1T>::SetVertexAttributes()
	{
		m_VertexAttributes = {
			{"Position", 3, offsetof(Vertex1P1N1T, Position)},
			{"Normal", 3, offsetof(Vertex1P1N1T, Normal)},
			{"TexCoords", 2, offsetof(Vertex1P1N1T, TexCoords)}
		};
	}
	
	template<>
	inline void VertexArray<Vertex1P1N1T4J>::SetVertexAttributes()
	{
		m_VertexAttributes = {
			{"Position", 3, offsetof(Vertex1P1N1T4J, Position)},
			{"Normal", 3, offsetof(Vertex1P1N1T4J, Normal)},
			{"TexCoords", 2, offsetof(Vertex1P1N1T4J, TexCoords)},
			{"BoneID", 4, offsetof(Vertex1P1N1T4J, BoneID)},
			{"Weight", 4, offsetof(Vertex1P1N1T4J, Weight)}
		};
	}

	template<typename VertexType>
	inline void VertexArray<VertexType>::SetLayout()
	{
		SetVertexAttributes();

		int index = 0; // Vertex attribute index
		for (const auto& attribute : m_VertexAttributes)
		{
			if (attribute.Name == "BoneID")
				glVertexAttribIPointer(index, attribute.NumOfComponents, GL_INT, sizeof(VertexType), (void*)attribute.Offset);
			else
				glVertexAttribPointer(index, attribute.NumOfComponents, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)attribute.Offset);
			glEnableVertexAttribArray(index);
			++index;
		}
	}

	template<typename VertexType>
	inline void VertexArray<VertexType>::AddLayout(int index, int numOfComponents, size_t stride, size_t offset)
	{
		//static size_t index = m_VertexAttributes.size(); // Next (free) vertex attribute index
		glVertexAttribPointer(index, numOfComponents, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(index);
		//++index;
	}

	template<typename VertexType>
	inline void VertexArray<VertexType>::SetDivisor(int vertexAttributeIndex, int divisor)
	{
		glVertexAttribDivisor(vertexAttributeIndex, divisor);
	}

	template<typename VertexType>
	inline void VertexArray<VertexType>::Bind()
	{
		glBindVertexArray(m_VertexArrayId);
	}

	template<typename VertexType>
	inline void VertexArray<VertexType>::Unbind()
	{
		glBindVertexArray(0);
	}

}