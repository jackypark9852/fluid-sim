#include <GLFW/glfw3.h> 
#include "mygl.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "iostream"

int main(void)
{
    MyGL myGL;
    myGL.InitializeGL();

    /* Loop until the user closes the window */
    while (!myGL.WindowShouldClose())
    {
        myGL.PaintGL(); 
    }

    glfwTerminate();
    return 0;
} 