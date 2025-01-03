#pragma once

#include <vector>

/// <summary>
/// Represents a source of density that can dynamically update over time.
/// This class is intended to be used as a base class for specific types of density sources.
/// </summary>
class DensitySource {
private:
    /// <summary>
    /// The array representing the amount of density to be added to the simulation per unit time.
    /// </summary>
    std::vector<double> source;

public:
    /// <summary>
    /// Updates the source array dynamically each physics frame.
    /// This function should be overridden by derived classes to implement specific behavior.
    /// </summary>
    virtual void Tick() = 0;

    /// <summary>
    /// Retrieves the current source array, which contains the density values to be added to the simulation.
    /// </summary>
    /// <returns>
    /// A constant reference to the source array containing density values.
    /// </returns>
    const std::vector<double>& GetSource();
};
