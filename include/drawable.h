#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm_includes.h"

#define dict std::unordered_map

enum BufferType : unsigned char {
    INDEX,
    POSITION, NORMAL, COLOR, UV,
    INTERLEAVED,
    INSTANCED_OFFSET,
    TRANS, INDEXTRANS
};

/// <summary>
/// Abstract class representing a drawable object.
/// Encapsulates the creation, management, and rendering of GPU buffer objects.
/// </summary>
class Drawable
{
protected:
    /// <summary>
    /// Map of buffer types to their OpenGL handles.
    /// Stores the handles for various buffers (e.g., vertex, index buffers).
    /// </summary>
    std::unordered_map<BufferType, GLuint> bufHandles;

    /// <summary>
    /// Map tracking whether a buffer of a specific type has been generated.
    /// True indicates the buffer is created and available for binding.
    /// </summary>
    std::unordered_map<BufferType, bool> bufGenerated;

    /// <summary>
    /// Map storing the length of index buffers by buffer type.
    /// Typically contains one key-value pair unless multiple index buffers are used.
    /// </summary>
    std::unordered_map<BufferType, int> indexCounts;

public:
    Drawable();
    virtual ~Drawable();

    /// <summary>
    /// Pure virtual function to create vertex buffer object (VBO) data.
    /// Must be implemented by subclasses to populate buffer data for rendering.
    /// </summary>
    virtual void CreateVBOdata() = 0;

    /// <summary>
    /// Frees the VBO data associated with this Drawable.
    /// Releases GPU resources allocated for buffers.
    /// </summary>
    virtual void DestroyVBOdata();

    /// <summary>
    /// Retrieves the OpenGL draw mode for this Drawable.
    /// Override in derived classes to specify the appropriate draw mode (e.g., GL_TRIANGLES).
    /// </summary>
    /// <returns>The OpenGL draw mode.</returns>
    virtual GLenum DrawMode() const;

    /// <summary>
    /// Gets the number of elements in the index buffer for the given buffer type.
    /// </summary>
    /// <param name="buf">The buffer type to query.</param>
    /// <returns>The number of elements in the index buffer.</returns>
    int ElemCount(BufferType buf) const;

    /// <summary>
    /// Generates an OpenGL buffer of the specified type.
    /// Initializes the buffer and updates the internal state of the Drawable.
    /// </summary>
    /// <param name="buf">The type of buffer to generate.</param>
    void GenerateBuffer(BufferType buf);

    /// <summary>
    /// Binds the OpenGL buffer of the specified type to the current context.
    /// </summary>
    /// <param name="buf">The type of buffer to bind.</param>
    /// <returns>True if the buffer was successfully bound; false otherwise.</returns>
    bool BindBuffer(BufferType buf) const;
};
