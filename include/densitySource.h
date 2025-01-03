#pragma once

#include <vector>

// Macro for accessing a 1D array with 2D-like syntax. This maps 2D indices (i, j) 
// to a 1D index in a flattened array. The grid includes a boundary, 
// so its actual dimensions are (N+2) x (N+2).
#define IX(i, j) ((i) + (N + 2) * (j))

/// <summary>
/// Represents a source of density that can dynamically update over time.
/// This class is intended to be used as a base class for specific types of density sources.
/// </summary>
class DensitySource {
protected:
    /// <summary>
    /// The width of the inner grid (non-boundary cells) excluding the boundary.
    /// The total grid dimensions are (N+2) x (N+2) to account for boundaries.
    /// </summary>
    unsigned int N;    

    /// <summary>
    /// The array representing the amount of density to be added to the simulation per unit time.
    /// </summary>
    std::vector<double> source;

public:
    /// <summary>
    /// Initializaes the source vector to the correct size.
    /// </summary>
    /// <param name="N"></param>
    DensitySource(unsigned int N);

    /// <summary>
    /// Updates the source array dynamically each physics frame.
    /// This function should be overridden by derived classes to implement specific behavior.
    /// </summary>
    virtual void Tick();

    /// <summary>
    /// Retrieves the current source array, which contains the density values to be added to the simulation.
    /// </summary>
    /// <returns>
    /// A constant reference to the source array containing density values.
    /// </returns>
    const std::vector<double>& GetSource() const;
};
