#pragma once
#include <GLFW/glfw3.h> 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class MyGL
{
private: 
	GLFWwindow* window; 
	ImGuiContext* imguiContext;
	unsigned int windowWidth; 
	unsigned int windowHeight; 

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
	void paintGL(); 
	
	/// <summary>
	/// Called whenever the window is resized. 
	/// Adjusts parameters dependent on window size such as perspective matrix. 
	/// </summary>
	/// <param name="windowWidth"> Window width after resizing </param>
	/// <param name="windowHeight"> Window height after resizing </param>
	void resizeGL(unsigned int windowWidth, unsigned int windowHeight);

	/// <summary>
	/// Cleans up and reinitialzes window and context 
	/// </summary>
	void resetGL(); 

	/// <summary>
	/// Cleans up resources related to this window and context
	/// </summary>
	void cleanUp(); 

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool windowShouldClose();
};

