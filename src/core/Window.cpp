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

    LOG_INFO(this);
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height) 
        {
            Window* w = (Window*)glfwGetWindowUserPointer(_window);
            LOG_INFO(&w);

            WindowResizeEvent e;
            e.Width = _width;
            e.Height = _height;

            w->OnEvent(e);
        });

    //

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");  

    m_IO = &(ImGui::GetIO());
}

void Window::Update()
{
    //TODO Move to renderer
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool demo = true;
    ImGui::ShowDemoWindow(&demo);

    if (ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(m_Width, m_Height));
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //
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
