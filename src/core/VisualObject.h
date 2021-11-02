#pragma once
#include "Buffer.h"
#include "VertexArray.h"
#include <glm.hpp>

class VisualObject
{
public:
	VisualObject() {};

	inline void SetObjectData(Buffer<float> v, Buffer<int> i, VertexArray* p)
	{
		m_VertexBuffer = v; m_IndexBuffer = i; m_VertexArray = p;
	}

	inline const glm::mat4 GetModelMatrix() { return m_ModelMatrix; }
	inline const Buffer<float>& GetVertexBuffer() { return m_VertexBuffer; }
	inline const Buffer<int>& GetIndexBuffer() { return m_IndexBuffer; }

private:

	Buffer<float> m_VertexBuffer;
	Buffer<int> m_IndexBuffer;

	VertexArray* m_VertexArray;					//TODO: remove if not used 

	glm::mat4 m_ModelMatrix;

};