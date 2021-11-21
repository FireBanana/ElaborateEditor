#include "ViewportRenderer.h"
#include "Logger.h"
#include "WindowResizeEvent.h"
#include "MousePositionEvent.h"
#include "MouseClickedEvent.h"

#include <GLFW/glfw3.h>

ViewportRenderer::ViewportRenderer(ViewportRenderData* data)
	: m_BackgroundShader("shaders\\vertex.vs", "shaders\\fragment.fs"),
	  m_DefaultShader("shaders\\default_vertex.vs", "shaders\\default_fragment.fs"),
	  m_RenderData(data), m_LastFrame(0), m_MouseButtonHeld(false), m_FirstMouseClick(false)
{
	m_DeltaTime = glfwGetTime();

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
			//Positions			  //Normals
			 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		}
	);

	cubeVertexBuffer.DefineAttribute({ 0, 3 });			//Positions
	cubeVertexBuffer.DefineAttribute({ 1, 3 });			//Normals
	cubeVertexBuffer.SetAttributes();

	m_Cube.SetObjectData(cubeVertexBuffer, cubeIndexBuffer, &m_3DVertexArray);

	m_DefaultShader.SetUniformFloat4("model", m_Cube.GetModelMatrix());
	m_DefaultShader.SetUniformFloat4("view", m_Camera.GetView());
	m_DefaultShader.SetUniformFloat4("projection", m_Camera.GetPerspective());

	//-----------------------------------------------------	
}

void ViewportRenderer::Render()
{
	m_DeltaTime = glfwGetTime() - m_LastFrame;
	m_LastFrame = glfwGetTime();

	//=====================================================

	m_2DVertexArray.Bind();

	m_BackgroundShader.Use();
	m_BackgroundShader.DrawIndexed(m_Viewport.GetVertexBuffer(), m_Viewport.GetIndexBuffer());

	glEnable(GL_DEPTH_TEST);

	m_3DVertexArray.Bind();	

	m_DefaultShader.Use();
	m_DefaultShader.Draw(m_Cube.GetVertexBuffer(), 36);

	glDisable(GL_DEPTH_TEST);
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

	eHandler.Dispatch<MouseClickedEvent>([&]() 
		{
			MouseClickedEvent* e = static_cast<MouseClickedEvent*>(&event);

			if (e->Button == GLFW_MOUSE_BUTTON_LEFT)
				if (e->Action == GLFW_PRESS)
				{
					m_MouseButtonHeld = true;
					m_FirstMouseClick = true;
				}
				else
					m_MouseButtonHeld = false;

		});

	eHandler.Dispatch<MousePositionEvent>([&]() 
		{
			if (!m_MouseButtonHeld)
				return;

			MousePositionEvent* e = static_cast<MousePositionEvent*>(&event);

			if (m_FirstMouseClick)
			{
				m_LastMousePosition.x = e->XPosition;
				m_LastMousePosition.y = e->YPosition;

				m_FirstMouseClick = false;
			}			
			
			m_MousePositionDelta.x = e->XPosition - m_LastMousePosition.x;
			m_MousePositionDelta.y = e->YPosition - m_LastMousePosition.y;

			m_LastMousePosition.x = e->XPosition;
			m_LastMousePosition.y = e->YPosition;

			m_Camera.Rotate(
				m_MousePositionDelta.x,
				m_MousePositionDelta.y, m_DeltaTime);
			
			m_DefaultShader.SetUniformFloat4("view", m_Camera.GetView());
		});
}
