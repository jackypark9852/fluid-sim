#include "drawable.h"

// Constructor
Drawable::Drawable() {}

// Destructor
Drawable::~Drawable() {}

// Pure virtual function to create vertex buffer object (VBO) data
void Drawable::createVBOdata() {}

// Frees the VBO data associated with this Drawable
void Drawable::destroyVBOdata() {}

// Retrieves the OpenGL draw mode for this Drawable
GLenum Drawable::drawMode() { return GL_TRIANGLES; }

// Gets the number of elements in the index buffer for the given buffer type
int Drawable::elemCount(BufferType buf) { return 0; }

// Generates an OpenGL buffer of the specified type
void Drawable::generateBuffer(BufferType buf) {}

// Binds the OpenGL buffer of the specified type to the current context
bool Drawable::bindBuffer(BufferType buf) { return false; }
