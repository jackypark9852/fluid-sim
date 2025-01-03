#include "circularSource.h"

CircularSource::CircularSource(unsigned int N, int x, int y, double radius, double amount):
	DensitySource(N), circleCenter(x, y), radius(radius), amount(amount),
	area(glm::pi<double>() * radius * radius)
{	
	// Lamda function for checking if the cell is within the radius from the circle center 
	auto InRadius = [&](int i, int j) {
		int a = i - circleCenter.x; 
		int b = j - circleCenter.y; 
		int c = radius; 
		return a * a + b * b < c * c; 
	};

	// Set the source matrix so that every density is added at every 
	// cell within the radius from the center. 
	double densPerCell = amount / area; 
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= N; ++j) {
			if (InRadius(i, j)) {
				source[IX(i, j)] = densPerCell; 
			}
		}
	}
}

void CircularSource::Tick()
{ 
	// Nothing should happen here because the sourcde does not change over ticks 
	return;
}
