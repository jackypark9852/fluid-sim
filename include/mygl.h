#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shaderprogram.h"
#include "quad.h"
#include "arrow.h"
#include "camera.h"
#include "ui/sceneselector.h"
#include <fluidsimulator.h>

class MyGL
{
private:
	unsigned int windowWidth;
	unsigned int windowHeight;

	GLFWwindow* window;
	ImGuiContext* imguiContext;
	GLuint vao;

	ShaderProgram overlayShader;
	ShaderProgram velFieldShader;

	Camera camera;
	/// <summary>
	/// Manages camera rotation, panning, and zoom.
	/// </summary>
	void updateCamera();

	Quad quad; /// <summary> A quad that covers the full screen (e.g. background) </summary>
	Arrow arrow; /// <summary> An arrow used for the velocity field display </summary>
	GLuint testTextureHandle; /// <summary> The handle for the test texture created in RenderTestTexture() </summary>
	GLuint velocityTextureHandle; /// <summary> The handle for the velocity field texture created in RenderVelocityField() </summary>
	FluidSimulator fluidSimulator;
	
	SceneSelector sceneSelector; /// <summary> An ImGui UI element for selecting the currently active scene in the simulationJKO </summary>
	
public: 
	// Rule of Three
	MyGL(unsigned int windowWidth = 1200, unsigned int windowHeight = 1200);
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
	/// Creates an ImGui window and draws debug ui
	/// </summary>
	void ShowImGuiWindow();
	
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

	/// <summary>
	/// Uses the velField shader program to render a velocity field
	/// </summary>
	void RenderVelocityField();

	/// <summary>
	/// Displays the velocity field's normal map
	/// </summary>
	void TestVelField();
};

