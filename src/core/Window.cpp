#include "Window.h"
#include "Logger.h"
#include "WindowResizeEvent.h"
#include "KeyPressedEvent.h"
#include "CharPressedEvent.h"

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

    //========= Set Events ================================================================

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
            if (_action != GLFW_PRESS || _mods != 0)
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

    // ====================================================================================
      
    m_ImguiRenderer = ImguiRenderer(m_Window, &m_RenderData);
    m_IO = &(ImGui::GetIO());
}

void Window::Update()
{
    m_ImguiRenderer.Render();
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

    eHandler.Dispatch<CharPressedEvent>([&]()
        {
            CharPressedEvent* e = static_cast<CharPressedEvent*>(&event);

            m_RenderData.GetCurrentLine().insert(
                m_RenderData.GetCurrentLine().begin() + m_RenderData.cursorPositionX,
                e->Key);

            m_RenderData.cursorPositionX++;
        });

    eHandler.Dispatch<KeyPressedEvent>([&]()
        {
            KeyPressedEvent* e = static_cast<KeyPressedEvent*>(&event);
        
            switch(e->Key)
            {
                case GLFW_KEY_BACKSPACE:

                    if (m_RenderData.cursorPositionX > 0)
                    {
                        m_RenderData.GetCurrentLine()
                            .erase(m_RenderData.GetCurrentLine().begin() + m_RenderData.cursorPositionX - 1);
                        
                        m_RenderData.cursorPositionX--;
                    }
                    else if (m_RenderData.lineNumber > 0)
                    {
                        m_RenderData.cursorPositionX = m_RenderData.GetLastLine().size();

                        m_RenderData.GetLastLine() += m_RenderData.GetCurrentLine();
                        //m_RenderData.DeleteCurrentLine();
                        m_RenderData.RetractAllLines(m_RenderData.lineNumber + 1);

                        m_RenderData.lineNumber--;
                    }

                    break;

                case GLFW_KEY_ENTER:

                    m_RenderData.lineNumber++;
                    m_RenderData.cursorPositionX = 0;
                    m_RenderData.textLines.insert({m_RenderData.lineNumber, ""});

                    break;

                case GLFW_KEY_LEFT:
                    
                    m_RenderData.cursorPositionX =
                        (int)m_RenderData.cursorPositionX - 1 >= 0 ?
                        (int)m_RenderData.cursorPositionX - 1 :
                        0;

                    break;

                case GLFW_KEY_RIGHT:

                    m_RenderData.cursorPositionX =
                        (int)m_RenderData.cursorPositionX + 1 <= m_RenderData.GetCurrentLine().size() ?
                        (int)m_RenderData.cursorPositionX + 1 :
                        m_RenderData.GetCurrentLine().size();

                    break;

                case GLFW_KEY_UP:

                    m_RenderData.lineNumber = 
                        (int)m_RenderData.lineNumber - 1 >= 0 ?
                        m_RenderData.lineNumber - 1 :
                        m_RenderData.lineNumber;

                    m_RenderData.cursorPositionX =
                        m_RenderData.cursorPositionX >= m_RenderData.GetCurrentLine().size() ?
                        m_RenderData.GetCurrentLine().size() :
                        m_RenderData.cursorPositionX;

                    break;

                case GLFW_KEY_DOWN:

                    m_RenderData.lineNumber = 
                        (int)m_RenderData.lineNumber + 1 < m_RenderData.textLines.size() ?
                        m_RenderData.lineNumber + 1 :
                        m_RenderData.lineNumber;

                    m_RenderData.cursorPositionX =
                        m_RenderData.cursorPositionX >= m_RenderData.GetCurrentLine().size() ?
                        m_RenderData.GetCurrentLine().size() :
                        m_RenderData.cursorPositionX;

                    break;
            }
            
        });
}
