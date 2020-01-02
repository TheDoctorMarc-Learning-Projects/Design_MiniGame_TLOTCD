#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#include <list>
#include <map>
#include "UiItem.h"
#include "UiItem_Image.h"
#include "UiItem_Label.h"

class j1Gui : public j1Module
{
public:

	j1Gui();
	~j1Gui();

 
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(); 
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public: 
	UiItem_Label* AddLabel(std::string name, std::string text, SDL_Color color, TTF_Font* font, p2Point<int> position, UiItem* const parent, float SpriteScale = 0.0f, bool resetable = false);
	UiItem_Image* AddImage(iPoint position, const SDL_Rect* section, std::string name, UiItem* const parent, float spriteScale = 0.0f,
		std::string newTextureName = "");

	UiItem* GetCurrentCanvas() const { 
		return currentCanvas; 
	};

	iPoint GetLastMousePos() const {
		return lastMousePos; 
	}
	UiItem* GetSelectedItem() const {
		return selectedItem; 
	}
	void SetSelectedItem(UiItem* item)
	{
		if (item == nullptr)
			return; 
		selectedItem = item; 
	}
 
	SDL_Texture* GetAtlas() const { return atlas; };
	float GetSpriteGlobalScale() const { return spriteScale; };   
	UiItem* GetCanvasItemByName(std::string name, UiItem* parent = nullptr) const;
	UiItem* GetItemByName(std::string name) const {
		for (const auto& item : listOfItems)
			if (item->name == name)
				return item; 

		return nullptr; 
	}

private:  
	void LoadXMLGUI(pugi::xml_node& menuNode);

	
private:
	SDL_Texture* atlas = nullptr;
	UiItem* currentCanvas = nullptr;
	std::list<UiItem*>	listOfItems;
	std::string atlas_file_name;
	float spriteScale = 0.0f; 
	iPoint lastMousePos = iPoint(0, 0); 
	UiItem* selectedItem = nullptr; 

public: 
	bool debug = false;

 
};






#endif


