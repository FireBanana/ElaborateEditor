#include "Window.h"
#include "Logger.h"
#include "WindowResizeEvent.h"

Window::Window(uint16_t width, uint16_t height)
    : m_Width(width), m_Height(height)
{
    if (!glfwInit())
        LOG_ERROR("Glfw failed to initialize");

    m_Window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

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

    // Set Events

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height) 
        {
            Window* w = (Window*)glfwGetWindowUserPointer(_window);

            WindowResizeEvent e;
            e.Width = _width;
            e.Height = _height;

            w->OnEvent(e);
        });

    //
      
    m_ImguiRenderer = ImguiRenderer(m_Window);
    m_IO = &(ImGui::GetIO());
}

void Window::Update()
{
    m_ImguiRenderer.Render(m_Width, m_Height);
}

void Window::OnEvent(Event& event)
{
    EventHandler eHandler(event);

    eHandler.Dispatch<WindowResizeEvent>([&]() 
        {
            WindowResizeEvent* e = static_cast<WindowResizeEvent*>(&event);
            m_Width = e->Width;
            m_Height = e->Height;
        });
}
