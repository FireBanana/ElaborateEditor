#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Id);
}
