#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>

#include "drawable.h"
#include "glm_includes.h"

class ShaderProgram
{
private:
    GLuint vertShader; ///< Handle for the vertex shader stored in this shader program
    GLuint fragShader; ///< Handle for the fragment shader stored in this shader program
    GLuint prog;       ///< Handle for the linked shader program stored in this class

public:
    /// <summary>
    /// Default constructor for ShaderProgram.
    /// Initializes the shader program with no attached shaders.
    /// </summary>
    ShaderProgram();

    /// <summary>
    /// Compiles and links the vertex and fragment shaders from the specified GLSL files.
    /// </summary>
    /// <param name="vertfile">Path to the vertex shader file.</param>
    /// <param name="fragfile">Path to the fragment shader file.</param>
    bool Create(const char* vertfile, const char* fragfile);

    /// <summary>
    /// Cleans up all GPU resources associated with this shader program.
    /// </summary>
    void Destroy();

    /// <summary>
    /// Activates this shader program in the OpenGL context for subsequent rendering.
    /// </summary>
    void useMe();

    /// <summary>
    /// Sets a 4x4 matrix uniform in the shader program.
    /// </summary>
    /// <param name="name">The name of the uniform variable in the shader.</param>
    /// <param name="m">The 4x4 matrix to be set.</param>
    void SetUnifMat4(std::string name, const glm::mat4& m);

    /// <summary>
    /// Sets a 2D vector uniform in the shader program.
    /// </summary>
    /// <param name="name">The name of the uniform variable in the shader.</param>
    /// <param name="v">The 2D vector to be set.</param>
    void SetUnifVec2(std::string name, const glm::vec2& v);

    /// <summary>
    /// Sets a 3D vector uniform in the shader program.
    /// </summary>
    /// <param name="name">The name of the uniform variable in the shader.</param>
    /// <param name="v">The 3D vector to be set.</param>
    void SetUnifVec3(std::string name, const glm::vec3& v);

    /// <summary>
    /// Sets a floating-point uniform in the shader program.
    /// </summary>
    /// <param name="name">The name of the uniform variable in the shader.</param>
    /// <param name="f">The float value to be set.</param>
    void SetUnifFloat(std::string name, float f);

    /// <summary>
    /// Sets an integer uniform in the shader program.
    /// </summary>
    /// <param name="name">The name of the uniform variable in the shader.</param>
    /// <param name="i">The integer value to be set.</param>
    void SetUnifInt(std::string name, int i);

    /// <summary>
    /// Sets an integer array uniform in the shader program at a specific offset.
    /// </summary>
    /// <param name="name">The name of the uniform array in the shader.</param>
    /// <param name="offset">The index offset in the array.</param>
    /// <param name="i">The integer value to be set.</param>
    void SetUnifArrayInt(std::string name, int offset, int i);

    /// <summary>
    /// Draws the specified drawable object using this shader program.
    /// </summary>
    /// <param name="drawable">The drawable object to render.</param>
    void Draw(const Drawable& drawable);

    /// <summary>
    /// Utility function that reads the content of a text file.
    /// </summary>
    /// <param name="file">The path to the text file.</param>
    /// <returns> A String of the file's contents </returns>
    std::string textFileRead(const char* file);

    /// <summary>
    /// Prints the log of any errors that occurred during shader compilation.
    /// </summary>
    /// <param name="shader">The handle of the shader to check.</param>
    void printShaderInfoLog(int shader);

    /// <summary>
    /// Prints the log of any errors that occurred during shader linking.
    /// </summary>
    /// <param name="prog">The handle of the shader program to check.</param>
    void printLinkInfoLog(int prog);
};
