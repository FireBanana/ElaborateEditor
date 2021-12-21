#include "Window.h"
#include "Logger.h"
#include "WindowResizeEvent.h"
#include "KeyPressedEvent.h"
#include "CharPressedEvent.h"
#include "MousePositionEvent.h"
#include "MouseClickedEvent.h"

Window::Window(uint16_t width, uint16_t height)
{
	if (!glfwInit())
		LOG_ERROR("Glfw failed to initialize");

	//Required for deploying on Mac
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_Window = glfwCreateWindow(width, height, "Elaborate Editor", NULL, NULL);

	if (!m_Window)
	{
		glfwTerminate();
		LOG_ERROR("Glfw failed to initialize");
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize opengl context");
	}

	glfwSetWindowUserPointer(m_Window, this);
	glViewport(width / 2.0f, 0, width / 2.0f, height);

	glEnable(GL_MULTISAMPLE);

	//========= Set Events ================================================================

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(_window);

			WindowResizeEvent e;
			e.Width = _width;
			e.Height = _height;

			w->OnEvent(e);
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* _window, int button, int action, int mods)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(_window);

			MouseClickedEvent e;
			e.Button = button;
			e.Action = action;
			e.Mods = mods;

			w->OnEvent(e);
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
		{
			if (_action == GLFW_RELEASE)
				return;

			Window* w = (Window*)glfwGetWindowUserPointer(_window);

			KeyPressedEvent e;
			e.Key = _key;
			e.Mods = _mods;

			w->OnEvent(e);
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* _window, unsigned int codepoint)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(_window);

			CharPressedEvent e;
			e.Key = codepoint;

			w->OnEvent(e);

		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* _window, double x, double y) 
		{
			Window* w = (Window*)glfwGetWindowUserPointer(_window);

			MousePositionEvent e;
			e.XPosition = x;
			e.YPosition = y;

			w->OnEvent(e);
		});

	// ====================================================================================

	m_ImguiRenderer = new ImguiRenderer(m_Window, &m_ImguiWindowData);
	m_ViewportRenderer = new ViewportRenderer(&m_ViewportRenderData);

	m_ImguiWindowData.window = this;
	m_ViewportRenderData.window = this;

	m_IO = &(ImGui::GetIO());
}

Window::~Window()
{
	delete m_ImguiRenderer;
	m_ImguiRenderer = nullptr;
}

void Window::Update()
{
	m_ViewportRenderer->Render();
	m_ImguiRenderer->Render();
}

void Window::OnEvent(Event& event)
{
	EventHandler eHandler(event);

	eHandler.Dispatch<WindowResizeEvent>([&]()
		{
			WindowResizeEvent* e = static_cast<WindowResizeEvent*>(&event);
			m_ImguiWindowData.width = e->Width;
			m_ImguiWindowData.height = e->Height;

			m_ViewportRenderData.width = e->Width;
			m_ViewportRenderData.height = e->Height;
		});

	m_ImguiRenderer->OnEvent(event);
	m_ViewportRenderer->OnEvent(event);
}
