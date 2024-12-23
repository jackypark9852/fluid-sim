#include "quad.h"

Quad::Quad() :
	Drawable() 
{}

void Quad::CreateVBOdata()
{
    GLuint idx[6]{ 0,1,2,0,2,3 };

    glm::vec4 vert_pos[4]{ glm::vec4(-1.f,-1.f,1.f,1.f),
                          glm::vec4(1.f,-1.f,1.f,1.f),
                          glm::vec4(1.f,1.f,1.f,1.f),
                          glm::vec4(-1.f,1.f,1.f,1.f) };

    glm::vec2 vert_UV[4]{ glm::vec2(0.f,0.f),
                         glm::vec2(1.f,0.f),
                         glm::vec2(1.f,1.f),
                         glm::vec2(0.f,1.f) };

    this->indexCounts[INDEX] = 6;

    this->GenerateBuffer(INDEX);
    this->BindBuffer(INDEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, idx, GL_STATIC_DRAW);

    this->GenerateBuffer(POSITION);
    this->BindBuffer(POSITION);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 4, vert_pos, GL_STATIC_DRAW);

    this->GenerateBuffer(UV);
    this->BindBuffer(UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 4, vert_UV, GL_STATIC_DRAW);
}
