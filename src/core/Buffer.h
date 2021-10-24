#pragma once
#include<glad/glad.h>
#include<vector>

#include "Logger.h"

struct VertexAttribute
{
	unsigned int Index;
	unsigned int Span;
};

template<typename T>
class Buffer
{
public:

	Buffer() : m_StrideSize(0) {};

	inline const unsigned int GetAttributeSize() const { return m_ElementSize; }
	inline const unsigned int GetSize() const { return m_VertexList.size(); }

	void Set(unsigned int bufferType);
	void Bind() const;
	void PushData(std::vector<T>&& vertices);
	void DefineAttribute(VertexAttribute attribute);
	void SetAttributes();

private:

	unsigned int m_Id;
	unsigned int m_BufferType;
	unsigned int m_ElementSize;
	unsigned int m_StrideSize;

	std::vector<VertexAttribute> m_AttributeList;
	std::vector<T> m_VertexList;
};

template<typename T>
void Buffer<T>::Set(unsigned int bufferType)
{
	m_BufferType = bufferType;

	glGenBuffers(1, &m_Id);
	Bind();
}

template<typename T>
void Buffer<T>::Bind() const
{
	glBindBuffer(m_BufferType, m_Id);
}

template<typename T>
void Buffer<T>::PushData(std::vector<T>&& vertices)
{
	m_VertexList = std::move(vertices);

	glBufferData(
		m_BufferType,
		m_VertexList.size() * sizeof(T),
		&m_VertexList[0],
		GL_STATIC_DRAW
	);
}

template<typename T>
void Buffer<T>::DefineAttribute(VertexAttribute attribute)
{
	m_AttributeList.push_back(attribute);
	m_ElementSize = m_AttributeList.size();
	m_StrideSize += attribute.Span;
}

template<typename T>
void Buffer<T>::SetAttributes()
{
	int currentSpan = 0;

	for (auto& item : m_AttributeList)
	{
		glVertexAttribPointer(
			item.Index, item.Span, GL_FLOAT, GL_FALSE,
			m_StrideSize * sizeof(float),
			(void*)(sizeof(float) * currentSpan));

		glEnableVertexAttribArray(item.Index);

		currentSpan += item.Span;
	}
}