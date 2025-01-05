#include "scenes/waterfountainscene.h"
#include "circularsource.h"
#include "rectvelocitysource.h"

WaterFountainScene::WaterFountainScene(unsigned int N, const std::string& name) :
    Scene(N, name)
{
    // Add a single density source to simulate the water particles
    // Positioned at the center of the bottom of the grid, with radius and intensity proportional to N
    densSources.push_back(CircularSource(N, N * 0.5, N * 0.8, N / 40.0, 30));

    // Add a velocity source that pushes particles upward, simulating the fountain effect
    // Positioned just above the density source, with a strong upward velocity proportional to N
    velSources.push_back(RectVelocitySource(N, N / 10, N / 10, N * 0.5 - N / 20, N * 0.8 - N / 20, 0.0, 0.15));

    // Add gravity to pull particles downward
    // Gravity source: A constant downward force on the particles, with a magnitude relative to N
    velSources.push_back(RectVelocitySource(N, N, N, 0, 0, 0.0, -0.05));
}
