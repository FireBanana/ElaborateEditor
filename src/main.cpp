#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Logger.h"

int main(void)
{
	Window win(1280, 720);

    while (!glfwWindowShouldClose(win.GetWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        win.Update();

        glfwSwapBuffers(win.GetWindow());
    }
}