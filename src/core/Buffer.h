#pragma once
#include<glad/glad.h>
#include<vector>

struct VertexAttribute
{
	unsigned int Index;
	unsigned int Span;
};

class Buffer
{
public:

	Buffer();

	void Bind();
	void PushData(std::vector<float>& vertices);
	void DefineAttribute(VertexAttribute& attribute);
	void SetAttributes();

private:

	unsigned int m_Id;
	std::vector<VertexAttribute> m_AttributeList;
};