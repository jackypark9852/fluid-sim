#include "drawable.h"

// Constructor
Drawable::Drawable() :
	bufHandles(), bufGenerated(), indexCounts()
{
	indexCounts[INDEX] = -1; 
}

// Destructor
Drawable::~Drawable() {
	destroyVBOdata(); 
}

// Frees the VBO data associated with this Drawable
void Drawable::destroyVBOdata() {
	for (auto& kvp : bufHandles) {
		glDeleteBuffers(1, &(kvp.second));
	}
}

// Retrieves the OpenGL draw mode for this Drawable
GLenum Drawable::drawMode() { return GL_TRIANGLES; }

// Gets the number of elements in the index buffer for the given buffer type
int Drawable::elemCount(BufferType buf) { return indexCounts[buf];}

// Generates an OpenGL buffer of the specified type
void Drawable::generateBuffer(BufferType buf) {
	bufGenerated[buf] = true;
	glGenBuffers(1, &bufHandles[buf]);
}

// Binds the OpenGL buffer of the specified type to the current context
bool Drawable::bindBuffer(BufferType buf) { 
	if (bufGenerated[buf]) {
		// Should you have more than one kind of index buffer,
		// make sure to update this conditional to include them.
		if (buf == INDEX || buf == INDEXTRANS) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufHandles[buf]); 
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, bufHandles[buf]);
		}	
	}
	return bufGenerated[buf];
}
