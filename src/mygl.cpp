#include <iostream>
#include "mygl.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

MyGL::MyGL(unsigned int windowWidth, unsigned int windowHeight): 
	windowWidth(windowWidth), windowHeight(windowHeight), 
    window(nullptr), imguiContext(nullptr), vao(0), 
    overlayShader()
{}

MyGL::MyGL(const MyGL& other):
	windowWidth(other.windowWidth), windowHeight(other.windowHeight), 
    window(nullptr), imguiContext(nullptr), vao(0),
    overlayShader()
{}

MyGL::~MyGL() {
	CleanUp(); 
}

MyGL& MyGL::operator=(const MyGL& other) 	
{
	windowWidth = other.windowWidth; 
	windowHeight = other.windowHeight;
	ResetGL(); 
    return *this; 
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

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); 

    // Set the user pointer for the window to this instance
    glfwSetWindowUserPointer(window, this);

    // Set framebuffer size callback
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        // Retrieve the MyGL instance associated with this window
        MyGL* instance = static_cast<MyGL*>(glfwGetWindowUserPointer(win));
        if (instance) {
            instance->ResizeGL(width, height);
        }
    });

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    if (!InitializeShaders()) {
        std::cerr << "Shader initialization failed" << std::endl; 
        exit(1); 
    }

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

void MyGL::PaintGL() {
    // Poll for and process events
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Example: Show ImGui demo window
    ImGui::ShowDemoWindow();

    overlayShader.useMe(); 
    int test = 3; 
    overlayShader.SetUnifInt("u_BlockType", test);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void MyGL::ResizeGL(unsigned int windowWidth, unsigned int windowHeight) {
    this->windowWidth = windowWidth; 
    this->windowHeight = windowHeight; 

    // TODO: Adjust persp matrix
}

void MyGL::ResetGL() {
	CleanUp(); 
	InitializeGL(); 
}

void MyGL::CleanUp() {
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

    // Destroy the compiled shaders 
    overlayShader.Destroy(); 
}

bool MyGL::WindowShouldClose() {
    return glfwWindowShouldClose(window); 
}

bool MyGL::InitializeShaders() {
    return overlayShader.Create("glsl/overlay.vert.glsl", "glsl/overlay.frag.glsl");
}