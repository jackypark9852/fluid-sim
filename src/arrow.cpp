#include "arrow.h"

Arrow::Arrow() :
	Drawable() 
{}

/// <summary>
/// An arrow is a rectangle with a triangle on top.
/// It has 7 vertices like so:
/// 
///        6 
///       / \ 
///      /   \  
///     5-3-2-4
///       | |
///       | |
///       0-1
/// 
/// </summary>
void Arrow::CreateVBOdata()
{

    GLuint idx[9]{0,1,2, 0,2,3, 4,5,6};

    glm::vec4 scaleDown(0.05, 0.05, 1, 1);

    glm::vec4 vert_pos[7]{ glm::vec4(-0.2f,-0.5f,1.f,1.f),
                          glm::vec4(0.2f,-0.5f,1.f,1.f),
                          glm::vec4(0.2f,0.25f,1.f,1.f),
                          glm::vec4(-0.2f,0.25f,1.f,1.f),
                          glm::vec4(0.5, 0.25, 1.f, 1.f),
                          glm::vec4(-0.5, 0.25, 1.f, 1.f),
                          glm::vec4(0.f, 0.8, 1.f, 1.f)};

    for (auto& vec : vert_pos) {
        vec *= scaleDown;
    }

    /*glm::vec2 vert_UV[4]{ glm::vec2(0.f,0.f),
                         glm::vec2(1.f,0.f),
                         glm::vec2(1.f,1.f),
                         glm::vec2(0.f,1.f) };*/

    this->indexCounts[INDEX] = 9;

    this->GenerateBuffer(INDEX);
    this->BindBuffer(INDEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 9, idx, GL_STATIC_DRAW);

    this->GenerateBuffer(POSITION);
    this->BindBuffer(POSITION);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 7, vert_pos, GL_STATIC_DRAW);

    /*this->GenerateBuffer(UV);
    this->BindBuffer(UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 4, vert_UV, GL_STATIC_DRAW);*/
}
