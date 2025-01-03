#pragma once

#include "velocitysource.h"
#include "glm_includes.h"

class RectVelocitySource : public VelocitySource {
private:
	/// <summary>
	/// The width of the rectangualr area where velocity is applied. 
	/// Clamped between [0, N] 
	/// </summary>
	int width;

	/// <summary>
	/// The height of the rectangualr area where velocity is applied. 
	/// Clamped between [0, N] 
	/// </summary>
	int height;

	/// <summary>
	/// The lower left position of the velocity source. 
	/// Clamped between [(0, 0), (N - 1, N - 1)]
	/// </summary>
	glm::ivec2 position; 

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	RectVelocitySource(unsigned int N, int width, int height, int x, int y, double uVel, double vVel);
};