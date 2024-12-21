#pragma once
#include <GLFW/glfw3.h> 

class MyGL
{
private: 
	GLFWwindow* window; 
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
	void InitializeGL(); 

	/// <summary>
	/// Clears the active window and renders scene and UI. 
	/// </summary>
	void paintGL(); 
	
	/// <summary>
	/// Called whenever window is resized. 
	/// </summary>
	void resizeGL(); 
};

