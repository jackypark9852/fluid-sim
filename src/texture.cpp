#include "texture.h"

Texture::Texture()
    : m_textureHandle(-1)
{}

Texture::~Texture()
{
    destroy();
}

void Texture::create(GLenum internalFormat,
    GLenum format)
{
    // Create a texture object on the GPU
    glGenTextures(1, &m_textureHandle);
    // Make that texture the "active texture" so that
    // any functions we call that operate on textures
    // operate on this one.
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);

    // Set the image filtering and UV wrapping options for the texture.
    // These parameters need to be set for EVERY texture you create.
    // They don't always have to be set to the values given here,
    // but they do need to be set.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // TODO: Port this to non-qt framework later when image loading function is required 
    
    // Only bother to set up a QImage to read the texture file
    // if there *is* a texture file. If there's not, then this
    // Texture is meant to be used as the write target for something
    // like a FrameBuffer.
    //if (textFileName) {
    //    // Create a QImage to load the image data
    //    // from the file indicated by `texturePath`
    //    QString path = QDir::currentPath();
    //    path = path.left(path.lastIndexOf("/"));
    //    path = path.left(path.lastIndexOf("/"));
    //    path = path.left(path.lastIndexOf("/"));
    //    path = path + "/textures" + textFileName;
    //    QImage img(path);
    //    img.convertTo(QImage::Format_ARGB32);
    //    img = img.mirrored();
    //    // Take the data stored in our QImage and send it to the GPU,
    //    // where it will be stored in the texture object we created with
    //    // glGenTextures.
    //    bufferPixelData(img.width(), img.height(),
    //        internalFormat, format, img.bits());
    //}
}


void Texture::bufferPixelData(unsigned int width, unsigned int height,
    GLenum internalFormat, GLenum format,
    GLvoid* pixels) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
        width, height,
        0, format, GL_UNSIGNED_BYTE, pixels);
}

void Texture::bind(int texSlot = 0)
{
    glActiveTexture(GL_TEXTURE0 + texSlot);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}

GLuint Texture::getHandle() const {
    return m_textureHandle;
}

void Texture::destroy() {
    glDeleteTextures(1, &m_textureHandle);
}
