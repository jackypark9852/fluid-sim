#include "scenes/scene.h"
#include "ui/sceneselector.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include  <iostream>		


SceneSelector::SceneSelector()
{}

void SceneSelector::ShowUI()
{
	std::cout << "Showing UI" << std::endl; 
	ImGui::Combo("MyCombo", &selectedSceneID, sceneNames.data(), sceneNames.size()); 
}

void SceneSelector::addScene(const Scene& scene) {
	sceneNames.push_back(scene.GetName()); 
}
