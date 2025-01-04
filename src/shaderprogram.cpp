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
bool ShaderProgram::Create(const char* vertfile, const char* fragfile) {
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
        return false;
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
        return false; 
    }
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(fragShader);
        return false; 
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
        return false; 
    }

    return true; 
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
    GLuint location = glGetUniformLocation(prog, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found or not used in shader program." << std::endl;
        return;
    }
    glUniform2fv(location, 1, glm::value_ptr(v));
}

// Sets a 3D vector uniform in the shader program
void ShaderProgram::SetUnifVec3(std::string name, const glm::vec3& v) {
    GLuint location = glGetUniformLocation(prog, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found or not used in shader program." << std::endl;
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(v));
}

// Sets a floating-point uniform in the shader program
void ShaderProgram::SetUnifFloat(std::string name, float f) {
    GLuint location = glGetUniformLocation(prog, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found or not used in shader program." << std::endl;
        return;
    }
    glUniform1f(location, f);
}

// Sets an integer uniform in the shader program
void ShaderProgram::SetUnifInt(std::string name, int i) {
    GLuint location = glGetUniformLocation(prog, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found or not used in shader program." << std::endl;
        return;
    }
    glUniform1i(location, i);
}

// Sets an integer array uniform in the shader program at a specific offset
void ShaderProgram::SetUnifArrayInt(std::string name, int offset, int i) {
    GLuint location = glGetUniformLocation(prog, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found or not used in shader program." << std::endl;
        return;
    }
    glUniform1iv(location + offset, 1, &i);  // Adjust location by offset
}

void ShaderProgram::Draw(const Drawable& drawable)
{
    if (drawable.ElemCount(INDEX) < 0) {
        throw std::invalid_argument(
            "Attempting to draw a Drawable that has not initialized its count variable! Remember to set it to the length of your index array in create()."
        );
    }
    useMe();

    GLint handle = glGetAttribLocation(prog, "vs_Pos");
    if (handle != -1 && drawable.BindBuffer(POSITION)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 4, GL_FLOAT, false, 0, nullptr);
    }

    handle = glGetAttribLocation(prog, "vs_Nor");
    if (handle != -1 && drawable.BindBuffer(NORMAL)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 4, GL_FLOAT, false, 0, nullptr);
    }

    handle = glGetAttribLocation(prog, "vs_Col");
    if (handle != -1 && drawable.BindBuffer(COLOR)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 4, GL_FLOAT, false, 0, nullptr);
    }

    handle = glGetAttribLocation(prog, "vs_UV");
    if (handle != -1 && drawable.BindBuffer(UV)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 2, GL_FLOAT, false, 0, nullptr);
    }


    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    drawable.BindBuffer(INDEX);
    glDrawElements(drawable.DrawMode(), drawable.ElemCount(INDEX), GL_UNSIGNED_INT, 0);
}

void ShaderProgram::DrawInstanced(const Drawable& drawable, int instances)
{
    if (drawable.ElemCount(INDEX) < 0) {
        throw std::invalid_argument(
            "Attempting to draw a Drawable that has not initialized its count variable! Remember to set it to the length of your index array in create()."
        );
    }
    useMe();

    GLint handle = glGetAttribLocation(prog, "vs_Pos");
    if (handle != -1 && drawable.BindBuffer(POSITION)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 4, GL_FLOAT, false, 0, nullptr);
    }

    handle = glGetAttribLocation(prog, "vs_Nor");
    if (handle != -1 && drawable.BindBuffer(NORMAL)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 4, GL_FLOAT, false, 0, nullptr);
    }

    handle = glGetAttribLocation(prog, "vs_Col");
    if (handle != -1 && drawable.BindBuffer(COLOR)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 4, GL_FLOAT, false, 0, nullptr);
    }

    handle = glGetAttribLocation(prog, "vs_UV");
    if (handle != -1 && drawable.BindBuffer(UV)) {
        glEnableVertexAttribArray(handle);
        glVertexAttribPointer(handle, 2, GL_FLOAT, false, 0, nullptr);
    }


    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    drawable.BindBuffer(INDEX);
    glDrawElementsInstanced(drawable.DrawMode(), drawable.ElemCount(INDEX), GL_UNSIGNED_INT, 0, instances);
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
