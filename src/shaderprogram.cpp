#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

// Default constructor
ShaderProgram::ShaderProgram()
    : vertShader(0), fragShader(0), prog(0)
{}

// Compiles and links the vertex and fragment shaders from the specified GLSL files
void ShaderProgram::Create(const char* vertfile, const char* fragfile) {
    // Allocate space on our GPU for a vertex shader and a fragment shader and a shader program to manage the two
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    prog = glCreateProgram();

    // Get the body of text stored in our two .glsl files
    std::string vertSourceStr = textFileRead(vertfile);
    std::string fragSourceStr = textFileRead(fragfile);

    if (vertSourceStr.empty() || fragSourceStr.empty()) {
        // Handle file reading failure appropriately
        std::cerr << "Failed to read shader source files" << std::endl;
        return;
    }

    const char* vertSource = vertSourceStr.c_str();
    const char* fragSource = fragSourceStr.c_str();

    // Send the shader text to OpenGL and store it in the shaders specified by the handles vertShader and fragShader
    glShaderSource(vertShader, 1, &vertSource, nullptr);
    glShaderSource(fragShader, 1, &fragSource, nullptr);

    // Tell OpenGL to compile the shader text stored above
    glCompileShader(vertShader);
    glCompileShader(fragShader);

    // Check if everything compiled OK
    GLint compiled;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(vertShader);
    }
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(fragShader);
    }

    // Tell prog that it manages these particular vertex and fragment shaders
    glAttachShader(prog, vertShader);
    glAttachShader(prog, fragShader);
    glLinkProgram(prog);

    // Check for linking success
    GLint linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        printLinkInfoLog(prog);
    }
}

// Cleans up all GPU resources associated with this shader program
void ShaderProgram::Destroy() {
    glDeleteProgram(prog);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

// Activates this shader program in the OpenGL context for subsequent rendering
void ShaderProgram::useMe() {
    glUseProgram(prog); 
}

// Sets a 4x4 matrix uniform in the shader program
void ShaderProgram::SetUnifMat4(std::string name, const glm::mat4& m) {
    GLuint matrix = glGetUniformLocation(prog, name.c_str());
    if (matrix == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found or not used in shader program." << std::endl;
        return;
    }
    glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(m));
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
std::string ShaderProgram::textFileRead(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf(); // Read the entire file into a string stream
    return ss.str();
}

// Prints the log of any errors that occurred during shader compilation
void ShaderProgram::printShaderInfoLog(int shader) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    // Additional OpenGL error checks should be included here

    if (infoLogLen > 0)
    {
        infoLog = new GLchar[infoLogLen];
        // Error check for failed memory allocation omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cout << "ShaderInfoLog:" << "\n" << infoLog << "\n";
        delete[] infoLog;
    }

    // Additional OpenGL error checks should be included here
}

// Prints the log of any errors that occurred during shader linking
void ShaderProgram::printLinkInfoLog(int prog) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    // Additional OpenGL error checks should be included here

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // Error check for failed memory allocation omitted
        glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        std::cout << "LinkInfoLog:" << "\n" << infoLog << "\n";
        delete[] infoLog;
    }
}
