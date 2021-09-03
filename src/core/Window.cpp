#include "Window.h"
#include "Logger.h"
#include "WindowResizeEvent.h"
#include "KeyPressedEvent.h"

Window::Window(uint16_t width, uint16_t height)
{
    if (!glfwInit())
        LOG_ERROR("Glfw failed to initialize");

    //Required for deploying on Mac
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
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

    // Set Events

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height) 
        {
            Window* w = (Window*)glfwGetWindowUserPointer(_window);

            WindowResizeEvent e;
            e.Width = _width;
            e.Height = _height;

            w->OnEvent(e);
        });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) 
        {
            if (_action != GLFW_PRESS)
                return;

            Window* w = (Window*)glfwGetWindowUserPointer(_window);

            KeyPressedEvent e;
            e.Key = _key;

            w->OnEvent(e);
        });

    //
      
    m_ImguiRenderer = ImguiRenderer(m_Window);
    m_IO = &(ImGui::GetIO());
}

void Window::Update()
{
    m_ImguiRenderer.Render(m_RenderData);
}

void Window::OnEvent(Event& event)
{
    EventHandler eHandler(event);

    eHandler.Dispatch<WindowResizeEvent>([&]() 
        {
            WindowResizeEvent* e = static_cast<WindowResizeEvent*>(&event);
            m_RenderData.width = e->Width;
            m_RenderData.height = e->Height;
        });

    eHandler.Dispatch<KeyPressedEvent>([&]()
        {
            KeyPressedEvent* e = static_cast<KeyPressedEvent*>(&event);
            m_RenderData.text += (char)e->Key;
        });
}
