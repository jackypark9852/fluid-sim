#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm_includes.h"
#include "drawable.h"

class Quad : public Drawable {
public:
    Quad();
    void createVBOdata() override;
};
