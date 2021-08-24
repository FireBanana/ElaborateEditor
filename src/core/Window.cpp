#include "Window.h"
#include "Logger.h"
#include "WindowResizeEvent.h"

Window::Window()
{
    if (!glfwInit())
        Logger::LOG_ERROR("Glfw failed to initialize");

    m_Window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!m_Window)
    {
        glfwTerminate();
        Logger::LOG_ERROR("Glfw failed to initialize");
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::LOG_ERROR("Failed to initialize opengl context");
    }

    glfwSetWindowUserPointer(m_Window, this);

    // Set Events

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
        {
            Window w = *(Window*)glfwGetWindowUserPointer(window);

            WindowResizeEvent e;
            e.Width = width;
            e.Height = height;

            w.OnEvent(e);
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
        ImGui::SetWindowSize(ImVec2(640, 480));
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //

    //Handle events
    //Logger::LOG_INFO(((WindowEvent*)glfwGetWindowUserPointer(m_Window))->type);
}

void Window::OnEvent(Event& event)
{
    EventHandler eHandler(event);

    eHandler.Dispatch<WindowResizeEvent>([&event]() 
        {
            //Logger::LOG_INFO(((WindowResizeEvent&)event).Width);
        std::cout << ((WindowResizeEvent&)event).Width << ", " << ((WindowResizeEvent&)event).Height << std::endl;
        });
}
