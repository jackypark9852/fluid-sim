#pragma once
#pragma once

#include <vector>

// Macro for accessing a 1D array with 2D-like syntax. This maps 2D indices (i, j) 
// to a 1D index in a flattened array. The grid includes a boundary, 
// so its actual dimensions are (N+2) x (N+2).
#define IX(i, j) ((i) + (N + 2) * (j))

/// <summary>
/// Represents a source of velocity that can dynamically update over time.
/// This class is intended to be used as a base class for specific types of velocity sources.
/// </summary>
class VelocitySource {
protected:
    /// <summary>
    /// The width of the inner grid (non-boundary cells) excluding the boundary.
    /// The total grid dimensions are (N+2) x (N+2) to account for boundaries.
    /// </summary>
    unsigned int N;

    /// <summary>
    /// The array representing the amount of horizontal velocity to be added to the simulation per unit time.
    /// </summary>
    std::vector<double> u;

    /// <summary>
    /// The array representing the amount of vertical velocity to be added to the simulation per unit time.
    /// </summary>
    std::vector<double> v;

    /// <summary>
    /// The magnitude of the horizontal velocity component (u) applied to the grid.
    /// Determines the speed and direction of horizontal movement in the simulation.
    /// Positive values indicate rightward movement, and negative values indicate leftward movement.
    /// </summary>
    double uVel;

    /// <summary>
    /// The magnitude of the vertical velocity component (v) applied to the grid.
    /// Determines the speed and direction of vertical movement in the simulation.
    /// Positive values indicate upward movement, and negative values indicate downward movement.
    /// </summary>
    double vVel;


public:
    /// <summary>
    /// Constructs a velocity source that dynamically adds velocity to the simulation
    /// based on the specified horizontal and vertical speed components.
    /// </summary>
    /// <param name="N">The width of the inner grid (excluding boundaries).</param>
    /// <param name="uVel">The magnitude of the horizontal velocity component to be added to the grid.</param>
    /// <param name="vVel">The magnitude of the vertical velocity component to be added to the grid.</param>
    VelocitySource(unsigned int N, double uVel, double vVel);

    /// <summary>
    /// Updates the source arrays dynamically each physics frame.
    /// This function should be overridden by derived classes to implement specific behavior.
    /// </summary>
    virtual void Tick();

    /// <summary>
    /// Retrieves the current horizontal velocity source array, which contains the density values to be added to the simulation.
    /// </summary>
    /// <returns>
    /// A constant reference to the source array containing density values.
    /// </returns>
    const std::vector<double>& GetHorizontalVelocitySource() const;

    /// <summary>
    /// Retrieves the current horizontal velocity source array, which contains the density values to be added to the simulation.
    /// </summary>
    /// <returns>
    /// A constant reference to the source array containing density values.
    /// </returns>
    const std::vector<double>& GetVerticalVelocitySource() const;
};
