#include "scenes/whirlwindscene.h"
#include "sources/circularsource.h"
#include "sources/rectvelocitysource.h"

WhirlwindScene::WhirlwindScene(unsigned int N, const std::string& name) :
	Scene(N, name, SceneType::TWOD)
{
	// Add two circular density sources that will simulate a swirling vortex
	// Source 1: Positioned near the center of the grid
	densSources.push_back(CircularSource(N, N / 2, N / 2, 20, 100));

	// Source 2: Positioned slightly above and to the right of the center
	densSources.push_back(CircularSource(N, N / 2 + 20, N / 2 - 20, 10, 50));

	// Add three rectangular velocity sources to create the whirlpool effect
	// Velocity Source 1: A clockwise rotation near the center to simulate swirling
	velSources.push_back(RectVelocitySource(N, 30, 30, N / 2 - 20, N / 2 - 20, 0.1, 0.1));

	// Velocity Source 2: A smaller force pushing particles outward from the center
	velSources.push_back(RectVelocitySource(N, 40, 40, N / 2 - 10, N / 2 - 10, -0.05, -0.05));

	// Velocity Source 3: A tangential force to reinforce the swirling movement
	velSources.push_back(RectVelocitySource(N, 30, 30, N / 2 + 50, N / 2, 0.1, -0.1));
}
