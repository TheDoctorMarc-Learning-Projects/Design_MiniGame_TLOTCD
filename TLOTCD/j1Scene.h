#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"
#include <list>
#include <map>
#include "SDL/include/SDL.h"

#define MAX_ALPHA 255

struct SDL_Texture;

enum class SceneState : int // the proper scene state
{
	MENU,
	GAME,
};

struct SDL_Color; 

class UiItem; 

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SaveLoadLogic(bool save, UiItem* callback = nullptr); 

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void SetSceneState(SceneState state) { this->state = state; }; 
	void SetPreviousSceneState(SceneState state) { this->previousSceneState = state; }; 

	SceneState GetPreviousSceneState() const { return previousSceneState; };
	SceneState GetCurrentSceneState() const { return state; }; 
	SceneState GetNextSceneState() const { return nextSceneState; }; 

private:
	SceneState state = SceneState::MENU;
	SceneState nextSceneState = SceneState::MENU;
	SceneState previousSceneState = SceneState::MENU;

	mutable SceneState savedState = SceneState::MENU;

	SDL_Color sceneSwapColor = { 30, 0, 0, 255 }; 
	bool loadGUI = false; 
	bool toUpdate = false; 

public:
	pugi::xml_node sceneNode;

	std::map<SceneState, std::string> sceneGuiXMLIndexes;
	std::map<SceneState, const char*> sceneMusics;   // will come in handy, as usual with maps :)
 
	void UnLoadScene(bool saveLoad = false);
	void LoadScene(SceneState sceneState, bool loadGUI, bool saveLoad = false);  
	void CreateScene();
	void SetTargetScene(SceneState sceneState)
	{
		nextSceneState = sceneState; 
		toUpdate = true; 
	}
};

#endif // __j1SCENE_H__