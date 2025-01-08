#include "sources\spheresource.h"

SphereSource::SphereSource(unsigned int N, int x, int y, int z, double radius, double amount) :
	DensitySource(N, SourceType::THREED), sphereCenter(x, y, z), radius(radius), amount(amount),
	area(glm::pi<double>()* radius* radius)
{
	// Lamda function for checking if the cell is within the radius from the circle center 
	auto InRadius = [&](int i, int j, int k) {
		int a = i - sphereCenter.x;
		int b = j - sphereCenter.y;
		int c = k - sphereCenter.z;
		return a * a + b * b + c * c < radius * radius;
		};

	// Set the source matrix so that every density is added at every 
	// cell within the radius from the center. 
	double densPerCell = amount / area;
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= N; ++j) {
			for (int k = 0; k <= N; ++k) {
				if (InRadius(i, j, k)) {
					source[IX3D(i, j, k)] = densPerCell;
				}
			}
		}
	}
}

void SphereSource::Tick()
{
	// Nothing should happen here because the sourcde does not change over ticks 
	return;
}
