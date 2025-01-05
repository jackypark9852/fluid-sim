#pragma once

#include "imgui.h"
#include "imguiui.h"
#include "scenes/scene.h"
#include <vector>

class SceneSelector : public ImguiUI {
private: 
	int selectedSceneID; 
	std::vector<const char*> sceneNames;

public: 
	SceneSelector();
	virtual void ShowUI() override; 
	void addScene(const Scene& scene); 
};