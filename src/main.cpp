#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

int main(void)
{
	Window win = Window::Window();

    while (!glfwWindowShouldClose(win.GetWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        win.Update();

        glfwSwapBuffers(win.GetWindow());
    }
}


