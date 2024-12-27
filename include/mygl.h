#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shaderprogram.h"
#include "quad.h"
#include "camera.h"

class MyGL
{
private: 
	unsigned int windowWidth;
	unsigned int windowHeight;

	GLFWwindow* window; 
	ImGuiContext* imguiContext;
	GLuint vao;

	ShaderProgram overlayShader;

	Camera camera;
	/// <summary>
	/// Manages camera rotation, panning, and zoom.
	/// </summary>
	void updateCamera();

	Quad quad; /// <summary> A quad that covers the full screen (e.g. background) </summary>
	GLuint testTextureHandle; /// <summary> The handle for the test texture created in RenderTestTexture() </summary>

public: 
	// Rule of Three
	MyGL(unsigned int windowWidth = 1920, unsigned int windowHeight = 1080);
	MyGL(const MyGL& other); 
	~MyGL();
	MyGL& operator=(const MyGL& other); 
	
	/// <summary>
	/// Creates an OpenGL window and compiles shader programs. 
	/// </summary>
	/// <returns> Whether intialization succeded </returns>
	bool InitializeGL(); 

	/// <summary>
	/// Clears the active window and renders scene and UI. 
	/// </summary>
	void PaintGL(); 
	
	/// <summary>
	/// Called whenever the window is resized. 
	/// Adjusts parameters dependent on window size such as perspective matrix. 
	/// </summary>
	/// <param name="windowWidth"> Window width after resizing </param>
	/// <param name="windowHeight"> Window height after resizing </param>
	void ResizeGL(unsigned int windowWidth, unsigned int windowHeight);

	/// <summary>
	/// Cleans up and reinitialzes window and context 
	/// </summary>
	void ResetGL(); 

	/// <summary>
	/// Cleans up resources related to this window and context
	/// </summary>
	void CleanUp(); 

	/// <summary>
	/// Checks whether the OpenGL window should close, typically triggered by user input or system events.
	/// </summary>
	/// <returns>True if the window should close, false otherwise.</returns>
	bool WindowShouldClose();

	/// <summary>
	/// Initializes the vertex and fragment shaders, compiles them, links them into a shader program, 
	/// and sets up any required shader variables or attributes.
	/// </summary>
	bool InitializeShaders();

	/// <summary>
	/// Uses the overlay shader program to render a test image to the full screen quad 
	/// </summary>
	void RenderTestImage(); 
};

