#include "scenes/whirlwindscene.h"
#include "circularsource.h"
#include "rectvelocitysource.h"
#include <glm/gtx/rotate_vector.hpp>

WhirlwindScene::WhirlwindScene(unsigned int N, const std::string& name) :
	Scene(N, name)
{
	// Add two circular density sources that will simulate a swirling vortex
	// Source 1: Positioned near the center of the grid
	// densSources.push_back(CircularSource(N, N / 2, N / 2, 20, 100));

	// Source 2: Positioned slightly above and to the right of the center
	densSources.push_back(CircularSource(N, N / 2 + 20, N / 2 - 20, 10, 50));

	std::vector<double> xVel((N + 2) * (N + 2), 0.0);
	std::vector<double> yVel((N + 2) * (N + 2), 0.0);
	for (int i = 1; i <= N; ++i) {
		for (int j = 1; j <= N; ++j) {
			double xCentered = i - (N + 2) * 0.5;
			double yCentered = j - (N + 2) * 0.5;
			double scalar = 0.002;
			glm::vec3 vel(-yCentered, xCentered, 0);
			vel *= scalar;
			vel = glm::rotateZ(vel, glm::radians(80.f));
			xVel[IX(i, j)] = vel.x;
			yVel[IX(i, j)] = vel.y;
			
		}
	}

	velSources.push_back(VelocitySource(N, xVel, yVel));
}
