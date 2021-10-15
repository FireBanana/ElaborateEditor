#include "Buffer.h"

Buffer::Buffer()
{
	glGenBuffers(1, &m_Id);
	Bind();
}

void Buffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void Buffer::PushData(std::vector<float>& vertices) //TODO change to allow all types
{
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices.size() * sizeof(float)),
		vertices.begin()._Ptr,
		GL_STATIC_DRAW
	);
}

void Buffer::DefineAttribute(VertexAttribute& attribute)
{
	m_AttributeList.push_back(attribute);
}

void Buffer::SetAttributes()
{
	int currentSpan = 0;

	for (auto& item : m_AttributeList)
	{
		glVertexAttribPointer(
			item.Index, item.Span, GL_FLOAT, GL_FALSE,
			m_AttributeList.size() * sizeof(float),
			(void*)(sizeof(float) * currentSpan));
		
		glEnableVertexAttribArray(1);

		currentSpan += item.Span;
	}
}
