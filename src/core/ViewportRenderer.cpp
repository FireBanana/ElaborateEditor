#include "ViewportRenderer.h"
#include "Logger.h"

ViewportRenderer::ViewportRenderer(uint16_t width, uint16_t height)
	: m_DefaultShader("C:\\Users\\mowai\\Desktop\\vertex.vs", "C:\\Users\\mowai\\Desktop\\fragment.fs")
{
	m_RenderData.width = width;
	m_RenderData.height = height;

	m_VertexArray.Bind();
	
	m_VertexBuffer.Set(GL_ARRAY_BUFFER);
	m_ElementBuffer.Set(GL_ELEMENT_ARRAY_BUFFER);	

	m_VertexBuffer.PushData(
		{
			-0.5f, 0.5f,
			 0.5f, 0.5f,
			-0.5f, -0.5f,
			 0.5f, -0.5f
		});

	m_ElementBuffer.PushData(
		{
			0, 1, 2, 1, 3, 2
		});	

	m_VertexBuffer.DefineAttribute({ 0, 2 });
	m_VertexBuffer.SetAttributes();

	m_DefaultShader.Use();
}

void ViewportRenderer::Render()
{
	m_DefaultShader.Draw(m_VertexBuffer, m_ElementBuffer);
}
