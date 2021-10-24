#pragma once

#include<glm.hpp>

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Event.h"

struct ViewportRenderData
{
	uint16_t width;
	uint16_t height;
	uint16_t x;
	uint16_t y;
};

class ViewportRenderer
{
public:
	ViewportRenderer(ViewportRenderData* data);

	void Render();
	void OnEvent(Event& event);

private:

	ViewportRenderData* m_RenderData;

	Buffer<float> m_VertexBuffer;
	Buffer<int> m_ElementBuffer;
	VertexArray m_VertexArray;
	Shader m_DefaultShader;
};