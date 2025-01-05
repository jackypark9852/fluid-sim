#include "scenes/scene.h"
#include "ui/sceneselector.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include  <iostream>		


SceneSelector::SceneSelector() :
	selectedSceneID(0), scenesStorage(), sceneNames()
{}

void SceneSelector::ShowUI()
{
    ImGui::Combo("Scenes", &selectedSceneID, sceneNames.data(), sceneNames.size());
}

void SceneSelector::AddScenes(const std::vector<std::string>& newSceneNames)
{
    // Append new scenes to the storage
    for (const std::string& sceneName : newSceneNames) {
        scenesStorage.push_back(sceneName);
    }

    // Rebuild the sceneNames vector because the pointers stored in sceneNames
    // are invalidated when scenesStorage is modified. This happens due to potential
    // reallocation of memory in scenesStorage (std::vector). Rebuilding ensures that
    // sceneNames contains valid pointers to the updated string data in scenesStorage.
    sceneNames.clear();
    for (const std::string& scene : scenesStorage) {
        sceneNames.push_back(scene.c_str());
    }
}

const std::string& SceneSelector::GetSelectedSceneName() const
{
	return scenesStorage[selectedSceneID];
}
