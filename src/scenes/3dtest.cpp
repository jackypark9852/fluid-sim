#include "scenes/3dtest.h"
#include "sources/circularsource.h"
#include "sources/rectvelocitysource.h"

ThreeDTest::ThreeDTest(unsigned int N, const std::string& name) :
	Scene(N, name, SceneType::THREED)
{
	// Add two circular density sources at opposite sides of the grid
	// Source 1: Positioned near the left edge
	densSources.push_back(CircularSource(N, N / 4, N / 2, 5, 100));

	// Source 2: Positioned near the right edge
	densSources.push_back(CircularSource(N, 3 * N / 4, N / 2, 5, 100));

	// Add two rectangular velocity sources to direct the smoke
	// Velocity Source 1: Pushes smoke from the left source to the right
	velSources.push_back(RectVelocitySource(N, 20, 20, N / 4 - 10, N / 2 - 10, 0.1, 0));

	// Velocity Source 2: Pushes smoke from the right source to the left
	velSources.push_back(RectVelocitySource(N, 20, 20, 3 * N / 4 - 10, N / 2 - 10, -0.1, 0));
}
