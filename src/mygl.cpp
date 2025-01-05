#include <iostream>
#include "mygl.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

MyGL::MyGL(unsigned int windowWidth, unsigned int windowHeight): 
	windowWidth(windowWidth), windowHeight(windowHeight), 
    window(nullptr), imguiContext(nullptr), vao(0), 
    overlayShader(), quad(), testTextureHandle(-1), velocityTextureHandle(-1),
    fluidSimulator(100, testTextureHandle, velocityTextureHandle),
    camera(windowWidth, windowHeight), sceneSelector(),
    velFieldShader(), arrow()
{
    sceneSelector.AddScenes(fluidSimulator.GetSceneNames()); 
}

MyGL::MyGL(const MyGL& other):
	windowWidth(other.windowWidth), windowHeight(other.windowHeight), 
    window(nullptr), imguiContext(nullptr), vao(0),
    overlayShader(), quad(), testTextureHandle(-1), velocityTextureHandle(-1),
    camera(windowWidth, windowHeight), sceneSelector(),
    velFieldShader(), arrow()
{
    sceneSelector.AddScenes(other.fluidSimulator.GetSceneNames());
}

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

    // Initialize vbo data for quad
    quad.CreateVBOdata(); 
    arrow.CreateVBOdata();

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
    // Update fluid sim
    fluidSimulator.Tick();

    // Poll for and process events
    glfwPollEvents();

    // Display debug ui
    ShowImGuiWindow();

    // Check which scene is selected by the user and set it as active scene in the fluid simulator
    fluidSimulator.ActivateSceneByName(sceneSelector.GetSelectedSceneName()); 

    // Handle mouse events for camera functions
    // TODO: move this somewhere more appropriate, but we don't have a Tick() yet().
    updateCamera();

    // Clear the screen
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    RenderTestImage();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void MyGL::ShowImGuiWindow()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Example: Show ImGui demo window
    //ImGui::ShowDemoWindow();
    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Fluid Simulator"))
    {
        //// Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    sceneSelector.ShowUI();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0 / (double)(ImGui::GetIO().Framerate), (double)(ImGui::GetIO().Framerate));
    ImGui::End();
}

void MyGL::ResizeGL(unsigned int windowWidth, unsigned int windowHeight) {
    this->windowWidth = windowWidth; 
    this->windowHeight = windowHeight; 

    // TODO: Adjust persp matrix
    camera.recomputeAspectRatio(windowWidth, windowHeight);
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
    velFieldShader.Destroy();
}

bool MyGL::WindowShouldClose() {
    return glfwWindowShouldClose(window); 
}

bool MyGL::InitializeShaders() {
    return overlayShader.Create("glsl/overlay.vert.glsl", "glsl/overlay.frag.glsl") 
        && velFieldShader.Create("glsl/velField.vert.glsl", "glsl/velField.frag.glsl");
}

void MyGL::RenderTestImage()
{
    if (testTextureHandle == -1) {
        const int textureWidth = 512;
        const int textureHeight = 512;

        // Create a gradient array
        std::vector<float> gradient(textureWidth * textureHeight * 4); // RGBA as floats
        for (int y = 0; y < textureHeight; ++y) {
            for (int x = 0; x < textureWidth; ++x) {
                int index = (y * textureWidth + x) * 4;
                gradient[index + 0] = x / double(textureWidth); // Red gradient
                gradient[index + 1] = y / double(textureHeight); // Green gradient
                gradient[index + 2] = 0.1f; // Constant Blue
                gradient[index + 3] = 1;
            }
        }


        // Generate and bind a texture
        glGenTextures(1, &testTextureHandle);
        glBindTexture(GL_TEXTURE_2D, testTextureHandle);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, gradient.data());

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        //Set fluid sim handle
        fluidSimulator.SetDensityTextureHandle(testTextureHandle);
    }

    // Render the textured quad
    overlayShader.useMe();

    // Bind the texture for rendering
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, testTextureHandle);
    overlayShader.SetUnifInt("u_Texture", 0);

    // Temporarily disable depth testing to ensure the full-screen quad is rendered 
    // without being affected by depth buffer comparisons.
    glDisable(GL_DEPTH_TEST);
    overlayShader.Draw(quad);
    glEnable(GL_DEPTH_TEST);

    if (ImGui::IsKeyDown(ImGuiKey_V)) {
        
        if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            TestVelField();
        }
        RenderVelocityField();
    }
}
void MyGL::TestVelField() {
    if (velocityTextureHandle == -1) {
        const int textureWidth = 512;
        const int textureHeight = 512;

        // Generate and bind a texture
        glGenTextures(1, &velocityTextureHandle);
        glBindTexture(GL_TEXTURE_2D, velocityTextureHandle);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Set fluid sim handle
        fluidSimulator.SetVelocityTextureHandle(velocityTextureHandle);
    }

    // render velocity field
    overlayShader.useMe();

    // Bind the texture for rendering
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, velocityTextureHandle);
    overlayShader.SetUnifInt("u_Texture", 1);
    glDisable(GL_DEPTH_TEST);
    // TODO: Set this 100 * 100 to a var that's connected to the fluidism
    overlayShader.Draw(quad);// , 100 * 100);
    glEnable(GL_DEPTH_TEST);
}

void MyGL::RenderVelocityField() {
    // create and bind texture
    if (velocityTextureHandle == -1) {
        const int textureWidth = 512;
        const int textureHeight = 512;

        // Generate and bind a texture
        glGenTextures(1, &velocityTextureHandle);
        glBindTexture(GL_TEXTURE_2D, velocityTextureHandle);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Set fluid sim handle
        fluidSimulator.SetVelocityTextureHandle(velocityTextureHandle);
    }

    // render velocity field
    velFieldShader.useMe();

    // Bind the texture for rendering
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, velocityTextureHandle);
    velFieldShader.SetUnifInt("u_Texture", 1);
    glDisable(GL_DEPTH_TEST);
    // TODO: Set this 100 * 100 to a var that's connected to the fluidism
    velFieldShader.DrawInstanced(arrow, 100 * 100);
    glEnable(GL_DEPTH_TEST);
}

void MyGL::updateCamera() {
    // Can add scalars to these values to adjust transform speeds
    if (!imguiContext) {
        return;
    }
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsMousePosValid() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        if (ImGui::IsMouseDown(0)) {
            camera.RotateAboutGlobalUp(ImGui::GetMouseDragDelta().x);
            camera.RotateAboutLocalRight(-ImGui::GetMouseDragDelta().y);
        }
        else if (ImGui::IsMouseDown(1)) {
            camera.PanAlongRight(-ImGui::GetMouseDragDelta().x);
            camera.PanAlongUp(ImGui::GetMouseDragDelta().y);
        }
        if (io.MouseWheel) {
            camera.Zoom(io.MouseWheel * 0.2f);
        }                 
    }
}