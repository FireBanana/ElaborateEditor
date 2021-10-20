#pragma once

#include<glm.hpp>

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"

struct ViewportRenderData
{
	uint16_t width;
	uint16_t height;
};

class ViewportRenderer
{
public:
	ViewportRenderer(uint16_t width, uint16_t height);

	void Render();

private:

	ViewportRenderData m_RenderData;

	Buffer<float> m_VertexBuffer;
	Buffer<int> m_ElementBuffer;
	VertexArray m_VertexArray;
	Shader m_DefaultShader;
};