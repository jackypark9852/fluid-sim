#include "scenes/scene.h"

Scene::Scene(unsigned int N, const std::string& name, SceneType type):
	N(N), name(name), sceneType(type)
{}

const std::string& Scene::GetName() const
{
	return name;
}

const std::vector<DensitySource>& Scene::GetDensSources() const
{
	return densSources; 
}

const std::vector<VelocitySource>& Scene::GetVelocitySources() const
{
	return velSources; 
}
