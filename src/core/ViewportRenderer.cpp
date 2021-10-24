#include "ViewportRenderer.h"
#include "Logger.h"
#include "WindowResizeEvent.h"

ViewportRenderer::ViewportRenderer(ViewportRenderData* data)
	: m_DefaultShader("viewport-shaders\\vertex.vs", "viewport-shaders\\fragment.fs"),
	m_RenderData(data)
{
	m_VertexArray.Bind();
	
	m_VertexBuffer.Set(GL_ARRAY_BUFFER);
	m_ElementBuffer.Set(GL_ELEMENT_ARRAY_BUFFER);	

	m_VertexBuffer.PushData(
		{
			 0.0f, 1.0f,
			 1.0f, 1.0f,
			 0.0f, -1.0f,
			 1.0f, -1.0f
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

void ViewportRenderer::OnEvent(Event& event)
{
	EventHandler eHandler(event);

	eHandler.Dispatch<WindowResizeEvent>([&]()
		{
			WindowResizeEvent* e = static_cast<WindowResizeEvent*>(&event);
			glViewport(0, 0, e->Width, e->Height);
		});
}
