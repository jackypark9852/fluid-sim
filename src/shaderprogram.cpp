#include "ShaderProgram.h"
#include <iostream>

// Default constructor
ShaderProgram::ShaderProgram()
    : vertShader(0), fragShader(0), prog(0)
{}

// Compiles and links the vertex and fragment shaders from the specified GLSL files
void ShaderProgram::Create(const char* vertfile, const char* fragfile) {
    // Implementation here
}

// Cleans up all GPU resources associated with this shader program
void ShaderProgram::Destroy() {
    // Implementation here
}

// Activates this shader program in the OpenGL context for subsequent rendering
void ShaderProgram::useMe() {
    // Implementation here
}

// Sets a 4x4 matrix uniform in the shader program
void ShaderProgram::SetUnifMat4(std::string name, const glm::mat4& m) {
    // Implementation here
}

// Sets a 2D vector uniform in the shader program
void ShaderProgram::SetUnifVec2(std::string name, const glm::vec2& v) {
    // Implementation here
}

// Sets a 3D vector uniform in the shader program
void ShaderProgram::SetUnifVec3(std::string name, const glm::vec3& v) {
    // Implementation here
}

// Sets a floating-point uniform in the shader program
void ShaderProgram::SetUnifFloat(std::string name, float f) {
    // Implementation here
}

// Sets an integer uniform in the shader program
void ShaderProgram::SetUnifInt(std::string name, int i) {
    // Implementation here
}

// Sets an integer array uniform in the shader program at a specific offset
void ShaderProgram::SetUnifArrayInt(std::string name, int offset, int i) {
    // Implementation here
}

// Draws the specified drawable object using this shader program
void ShaderProgram::Draw(const Drawable& drawable) {
    // Implementation here
}

// Utility function that reads the content of a text file
char* ShaderProgram::textFileRead(const char* file) {
    // Implementation here
    return nullptr; // Placeholder return
}

// Prints the log of any errors that occurred during shader compilation
void ShaderProgram::printShaderInfoLog(int shader) {
    // Implementation here
}

// Prints the log of any errors that occurred during shader linking
void ShaderProgram::printLinkInfoLog(int prog) {
    // Implementation here
}
