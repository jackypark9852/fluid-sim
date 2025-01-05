#pragma once

#include "imgui.h"
#include "imguiui.h"
#include "scenes/scene.h"
#include <vector>
#include <string>

class SceneSelector : public ImguiUI {
private: 
	std::vector<std::string> scenesStorage; // Stores the actual string data
	std::vector<const char*> sceneNames;    // Stores pointers to the strings
	int selectedSceneID;					// Selected scene index

public: 
	/// <summary>
	/// Constructs a SceneSelector object
	/// </summary>
	SceneSelector();

	/// <summary>
	/// Displays an ImGui combo for selecting scenes. 
	/// ImGui window must be initialized before calling this function.
	/// </summary>
	virtual void ShowUI() override; 
	
	/// <summary>
	/// Adds scenes to the scene selector list. 
	/// </summary>
	/// <param name="scene"> The vector containing string literals of the scenes' names. </param>
	void AddScenes(const std::vector<std::string>& sceneNames);

	/// <summary>
	/// Retrieves the name of the selected scene. 
	/// </summary>
	/// <returns> A char array containing the name of the selecteed scene. </returns>
	const std::string& GetSelectedSceneName() const; 
};