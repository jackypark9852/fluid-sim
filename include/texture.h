#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> a

class MyGL;

class Texture
{
private:
    GLuint m_textureHandle;

public:
    Texture();
    ~Texture();

    void create(GLenum internalFormat = GL_RGBA,
        GLenum format = GL_BGRA);
    void bufferPixelData(unsigned int width, unsigned int height,
        GLenum internalFormat, GLenum format, GLvoid* pixels);
    void bind(int texSlot);
    void destroy();

    GLuint getHandle() const;

};
