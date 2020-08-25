#pragma once
#include "Core/Common.h"
#include "Vertex.h"

namespace Yugo
{
	/*
	This class is templated Buffer class that can hold any data type (e.g. custom Vertex type, unsigned int...)
	*/
	template<typename Type>
	class Buffer
	{
	private:
		uint32_t m_BufferId;
	
	public:
		Buffer();
		~Buffer();

		void SetData(const std::vector<Type>& data);

		void Bind();
		void Unbind();
	};

	template<typename Type>
	inline Buffer<Type>::Buffer()
	{
		glGenBuffers(1, &m_BufferId);
	}

	template<typename Type>
	inline Buffer<Type>::~Buffer()
	{
		glDeleteBuffers(1, &m_BufferId);
	}

	template<typename Type>
	inline void Buffer<Type>::SetData(const std::vector<Type>& data)
	{
		// If type is unsigned int, then it is Index/Element buffer (GL_ELEMENT_ARRAY_BUFFER)
		if (typeid(Type) == typeid(unsigned int))
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(Type), &data[0], GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Type), &data[0], GL_STATIC_DRAW);

	}

	template<typename Type>
	inline void Buffer<Type>::Bind()
	{
		if (typeid(Type) == typeid(unsigned int))
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
		else
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	template<typename Type>
	inline void Buffer<Type>::Unbind()
	{
		if (typeid(Type) == typeid(unsigned int))
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		else
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	class FrameBuffer
	{
	private:
		uint32_t m_BufferId;

	public:
		enum class AttachmentType
		{
			TextureBuffer,
			RenderBuffer
		};

		FrameBuffer();
		~FrameBuffer();
		uint32_t GetId() const;
		void Bind();
		void Unbind();
		void Attach(uint32_t bufferId, AttachmentType type);
	};


	class RenderBuffer
	{
	private:
		uint32_t m_BufferId;

	public:
		RenderBuffer();
		~RenderBuffer();
		uint32_t GetId() const;
		void Bind();
		void Unbind();
		void Storage(int width, int height);
		void Resize(int width, int height);
	};

}