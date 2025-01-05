#pragma once

#include "scenes/scene.h"
#include "circularsource.h"
#include "rectvelocitysource.h"

class WhirlwindScene : public Scene {
public: 
	WhirlwindScene(unsigned int N = 1000, const std::string& name = "Whirlwind Scene"); 
};