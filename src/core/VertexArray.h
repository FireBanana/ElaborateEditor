#pragma once
#include <glad/glad.h>

class VertexArray
{
public:
	VertexArray();

	void Bind();
private:

	unsigned int m_Id;

};