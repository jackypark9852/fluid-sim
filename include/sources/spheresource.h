#pragma once

#include "densitySource.h"
#include "glm_includes.h"

/// <summary>
/// Represents a circular density source that adds density to the simulation
/// within a specified radius and at a specified rate.
/// </summary>
class SphereSource : public DensitySource {
private:
    /// <summary>
    /// The center of the circular source in grid coordinates.
    /// </summary>
    glm::ivec3 sphereCenter;

    /// <summary>
    /// The radius of the circular source, defining the area affected by the source.
    /// </summary>
    double radius;

    /// <summary>
    /// The density amount to be added per unit area within the source's radius.
    /// </summary>
    double amount;

    /// <summary>
    /// The total area of the sphere source, computed from the radius.
    /// </summary>
    double area;

public:
    /// <summary>
    /// Constructs a circular density source.
    /// </summary> 
    /// <param name="N">The width of the simulated grid</param>
    /// <param name="y">The y-coordinate of the center of the circular source.</param>
    /// <param name="radius">The radius of the source, determining its area of influence.</param>
    /// <param name="amount">The density amount to be added per unit area within the source's radius.</param>
    SphereSource(unsigned int N, int x, int y, int z, double radius, double amount);

    /// <summary>
    /// Updates the density source each physics frame.
    /// This function modifies the source array to reflect the current state of the circular source.
    /// </summary>
    void Tick() override;
};
