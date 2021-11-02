#include "ViewportRenderer.h"
#include "Logger.h"
#include "WindowResizeEvent.h"

ViewportRenderer::ViewportRenderer(ViewportRenderData* data)
	: m_BackgroundShader("shaders\\vertex.vs", "shaders\\fragment.fs"),
	  m_DefaultShader("shaders\\default_vertex.vs", "shaders\\default_fragment.fs"),
	  m_RenderData(data)
{
	//-----------------------------------------------------
	m_2DVertexArray.Bind();

	Buffer<float> viewportVertexBuffer;
	Buffer<int> viewportIndexBuffer;

	viewportVertexBuffer.Set(GL_ARRAY_BUFFER);
	viewportIndexBuffer.Set(GL_ELEMENT_ARRAY_BUFFER);

	viewportVertexBuffer.PushData(
		{
			 -1.0f, -1.0f,
			 1.0f, -1.0f,
			 -1.0f, 1.0f,
			 1.0f, 1.0f
		});

	viewportIndexBuffer.PushData(
		{
			0, 1, 2,
			1, 3, 2
		});	

	viewportVertexBuffer.DefineAttribute({ 0, 2 });
	viewportVertexBuffer.SetAttributes();

	m_Viewport.SetObjectData(viewportVertexBuffer, viewportIndexBuffer, &m_2DVertexArray);

	//-----------------------------------------------------

	m_3DVertexArray.Bind();

	Buffer<float> cubeVertexBuffer;
	Buffer<int> cubeIndexBuffer;

	cubeVertexBuffer.Set(GL_ARRAY_BUFFER);
	cubeIndexBuffer.Set(GL_ELEMENT_ARRAY_BUFFER);

	cubeVertexBuffer.PushData(
		{
			//Vertex Positions
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f			
		}
	);

	cubeIndexBuffer.PushData(
		{
			0, 1, 2,
			2, 0, 3,
			3, 0, 5,
			5, 3, 4,
			4, 5, 6,
			6, 4, 7,
			7, 6, 1,
			1, 7, 2,
			1, 6, 5,
			5, 1, 0,
			3, 4, 2,
			2, 7, 4
		}
	);

	cubeVertexBuffer.DefineAttribute({0, 3});
	cubeVertexBuffer.SetAttributes();

	m_Cube.SetObjectData(cubeVertexBuffer, cubeIndexBuffer, &m_3DVertexArray);

	m_DefaultShader.SetUniformFloat4("model", m_Cube.GetModelMatrix());
	m_DefaultShader.SetUniformFloat4("view", m_Camera.GetView());
	m_DefaultShader.SetUniformFloat4("projection", m_Camera.GetPerspective());

	//-----------------------------------------------------

	
}

void ViewportRenderer::Render()
{
	m_2DVertexArray.Bind();
	m_BackgroundShader.Use();
	m_BackgroundShader.Draw(m_Viewport.GetVertexBuffer(), m_Viewport.GetIndexBuffer());

	m_3DVertexArray.Bind();
	m_DefaultShader.Use();
	m_DefaultShader.Draw(m_Cube.GetVertexBuffer(), m_Cube.GetIndexBuffer());
}

void ViewportRenderer::OnEvent(Event& event)
{
	EventHandler eHandler(event);

	eHandler.Dispatch<WindowResizeEvent>([&]()
		{
			WindowResizeEvent* e = static_cast<WindowResizeEvent*>(&event);

			glViewport(e->Width / 2.0f, 0, e->Width / 2.0f, e->Height);
			m_Camera.UpdateAspectRatio((e->Width / 2.0f) / e->Height);
			m_DefaultShader.SetUniformFloat4("projection", m_Camera.GetPerspective());
		});
}
