#include <iostream>
#include "mygl.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

MyGL::MyGL(unsigned int windowWidth, unsigned int windowHeight): 
	windowWidth(windowWidth), windowHeight(windowHeight) 
{}

MyGL::MyGL(const MyGL& other):
	windowWidth(other.windowWidth), windowHeight(other.windowHeight)
{}

MyGL::~MyGL() {
	cleanUp(); 
}

MyGL& MyGL::operator=(const MyGL& other) 	
{
	windowWidth = other.windowWidth; 
	windowHeight = other.windowHeight;
	resetGL(); 
}

bool MyGL::InitializeGL() {
    // Initialize GLFW (only once globally)
    static bool glfwInitialized = false;
    if (!glfwInitialized) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return false;
        }
        glfwInitialized = true;
    }

    // Create the GLFW window
    window = glfwCreateWindow(windowWidth, windowHeight, "Fluid Simulator!", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    // Make the OpenGL context current for this window
    glfwMakeContextCurrent(window);

    // Set the user pointer for the window to this instance
    glfwSetWindowUserPointer(window, this);

    // Set framebuffer size callback
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        // Retrieve the MyGL instance associated with this window
        MyGL* instance = static_cast<MyGL*>(glfwGetWindowUserPointer(win));
        if (instance) {
            instance->resizeGL(width, height);
        }
    });

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui context (unique per instance)
    IMGUI_CHECKVERSION();
    imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);

    // Configure ImGui IO
    ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Install GLFW callbacks
    ImGui_ImplOpenGL3_Init();

    return true;
}

void MyGL::paintGL() {
    // Poll for and process events
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Example: Show ImGui demo window
    ImGui::ShowDemoWindow();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void MyGL::resizeGL(unsigned int windowWidth, unsigned int windowHeight) {
    this->windowWidth = windowWidth; 
    this->windowHeight = windowHeight; 

    // TODO: Adjust persp matrix
}

void MyGL::resetGL() {
	cleanUp(); 
	InitializeGL(); 
}

void MyGL::cleanUp() {
    // Terminate ImGui context for this instance (if it exists)
    if (imguiContext) {
        ImGui::SetCurrentContext(imguiContext); // Ensure this instance's context is current
        ImGui::DestroyContext(imguiContext);
        imguiContext = nullptr; // Clear the stored context
    }

    // Destroy the specific GLFW window
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

bool MyGL::windowShouldClose() {
    return glfwWindowShouldClose(window); 
}