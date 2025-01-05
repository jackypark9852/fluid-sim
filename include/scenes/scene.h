#pragma once

#include "densitysource.h"
#include "velocitysource.h"

/// <summary>
/// A class representing a scene in the simulation. 
/// A scene is composed of components that alter the simulation, such as 
/// density sourrces and velocity sources. 
/// </summary>
class Scene {
private:
	// The name of the scene
	const char* name; 

	// Density sources present in the current simulation 
	std::vector<DensitySource> densSources;

	// Velocity sources present in the current simulation 
	std::vector<VelocitySource> velSources; 

public:
	/// <summary>
	/// Constructs a scene object
	/// </summary>
	/// <param name="name"> The name of the scene </param>
	Scene(const char* name = "Empty Scene");

	/// <summary>
	/// Returns the name of the string.
	/// </summary>
	/// <returns> A string literal of the name of the scene. </returns>
	const char* GetName() const; 

	/// <summary>
	/// Returns the density sources in the scene.
	/// </summary>
	/// <returns> A constant DensitySource vector reference containing the density sources in the scene. </returns>
	const std::vector<DensitySource> GetDensSources() const; 

	/// <summary>
	/// Returns the velocity sources in the kscene.
	/// </summary>
	/// <returns> A constant VelocitySource vector reference containing the velocity sources in the scene. </returns>
	const std::vector<VelocitySource> GetVelocitySources() const; 
};