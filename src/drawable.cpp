#include "drawable.h"

// Constructor
Drawable::Drawable() :
	bufHandles(), bufGenerated(), indexCounts()
{
	indexCounts[INDEX] = -1; 
}

// Destructor
Drawable::~Drawable() {
	DestroyVBOdata(); 
}

// Frees the VBO data associated with this Drawable
void Drawable::DestroyVBOdata() {
	for (auto& kvp : bufHandles) {
		glDeleteBuffers(1, &(kvp.second));
	}
}

// Retrieves the OpenGL draw mode for this Drawable
GLenum Drawable::DrawMode() const { return GL_TRIANGLES; }

// Gets the number of elements in the index buffer for the given buffer type
int Drawable::ElemCount(BufferType buf) const { 
	// Return an error value if the buffer of this type has not been created
	if (indexCounts.count(buf) == 0) {
		return -1; 
	}
	return indexCounts.at(buf);
}

// Generates an OpenGL buffer of the specified type
void Drawable::GenerateBuffer(BufferType buf) {
	bufGenerated[buf] = true;
	glGenBuffers(1, &bufHandles[buf]);
}

// Binds the OpenGL buffer of the specified type to the current context
bool Drawable::BindBuffer(BufferType buf) const { 
	if (bufGenerated.at(buf)) {
		// Should you have more than one kind of index buffer,
		// make sure to update this conditional to include them.
		if (buf == INDEX || buf == INDEXTRANS) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufHandles.at(buf)); 
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, bufHandles.at(buf));
		}	
	}
	return bufGenerated.at(buf);
}
