#include "scenes/scene.h"

Scene::Scene(const char* name):
	name(name)
{}

const char* Scene::GetName() const
{
	return name;
}

const std::vector<DensitySource> Scene::GetDensSources() const
{
	return densSources; 
}

const std::vector<VelocitySource> Scene::GetVelocitySources() const
{
	return velSources; 
}
