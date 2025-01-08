#pragma once

#include "sources/densitysource.h"
#include "sources/velocitysource.h"
#include <string>

enum class SceneType {
	NONE = 0,
	TWOD,
	THREED
};

/// <summary>
/// A class representing a scene in the simulation. 
/// A scene is composed of components that alter the simulation, such as 
/// density sourrces and velocity sources. 
/// </summary>
class Scene {
protected:
	// The width of the inner grid (non-boundary cells) excluding the boundary.
	// The total grid dimensions are (N+2) x (N+2) to account for boundaries.
	SceneType sceneType;
	
	unsigned int N;

	// The name of the scene
	std::string name; 

	// Density sources present in the current simulation 
	std::vector<DensitySource> densSources;

	// Velocity sources present in the current simulation 
	std::vector<VelocitySource> velSources; 

public:
	/// <summary>
	/// Constructs a scene object
	/// </summary>
	/// <param name="name"> The name of the scene </param>
	Scene(unsigned int N = 1000, const std::string& name = "Empty Scene", SceneType type = SceneType::TWOD);

	/// <summary>
	/// Returns the name of the string.
	/// </summary>
	/// <returns> A string literal of the name of the scene. </returns>
	const std::string& GetName() const; 

	/// <summary>
	/// Returns the density sources in the scene.
	/// </summary>
	/// <returns> A constant DensitySource vector reference containing the density sources in the scene. </returns>
	const std::vector<DensitySource>& GetDensSources() const; 

	/// <summary>
	/// Returns the velocity sources in the kscene.
	/// </summary>
	/// <returns> A constant VelocitySource vector reference containing the velocity sources in the scene. </returns>
	const std::vector<VelocitySource>& GetVelocitySources() const; 
};