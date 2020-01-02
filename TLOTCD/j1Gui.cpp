#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1EntityFactory.h"
#include "j1FadeToBlack.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"
#include "j1Fonts.h"

j1Gui::j1Gui() : j1Module()
{
	name.assign("gui");
}



j1Gui::~j1Gui()
{

}

bool j1Gui::Awake(pugi::xml_node& conf)
{
	bool ret = true;
	atlas_file_name = conf.child("atlas").attribute("file").as_string();
	spriteScale = conf.child("spriteScale").attribute("value").as_float();

	currentCanvas = DBG_NEW UiItem();
	LoadXMLGUI((pugi::xml_node&) conf);
	return true;
}

bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.data());

	// load the main menu GUI 

	App->audio->PlayMusic("/sound/music/soil-halo.ogg");

	return true;
}

bool j1Gui::PreUpdate()
{
	BROFILER_CATEGORY("GUI PreUpdate", Profiler::Color::Gainsboro);

	
	for (auto& item : currentCanvas->GetChildrenRecursive())
		if (item->enable == true)
			if (item->targPosData.toUpdatePos == true)
			{
				item->hitBox.x += item->targPosData.speed.x;
				item->hitBox.y += item->targPosData.speed.y;

				if (item->GetPos() == item->targPosData.targetPos)
					item->targPosData.toUpdatePos = false;
			}
	



	return true; 
}

bool j1Gui::Update(float dt)
{
	BROFILER_CATEGORY("GUI Update", Profiler::Color::Gainsboro);

		this->dt = dt;
	iPoint mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);
	uint mouseState = App->input->GetMouseButtonDown(SDL_BUTTON_LEFT);


    // go through all current canvas family tree 
	for (auto& item : currentCanvas->GetChildrenRecursive())
	{
		if (item->enable == false || item->numb == true)
			continue; 

		if (App->entityFactory->isPointInsideRect(&mousePos, &item->hitBox) == true)   // MOUSE INSIDE HITBOX
		{


			if (item->state == DRAG)
				item->OnDrag(mousePos);

			if (item->state != CLICK && mouseState == KEY_DOWN)
			{
				
				if (selectedItem == nullptr && item->accionable == true)
				{
					item->state = CLICK;
					selectedItem = item;
					item->OnClickDown(mousePos);
				}
					
			}

			if (item->state != DRAG && mouseState == KEY_REPEAT)
			{
				if (selectedItem == item)
				{
					if (item->draggable == true)
					{
						item->state = DRAG;
						item->OnDrag(mousePos);
					}
						
				}
				
			}


			if ((item->state == CLICK || item->state == DRAG) && mouseState == KEY_UP)
			{
				if (item->state == DRAG && item->focusable == false)
					item->SetOriginPos(); 

				item->state = HOVER;
				selectedItem = nullptr;
				item->OnClickUp();
			
			}

			else if (item->state == IDLE)
			{
				item->OnHover(); 
				item->state = HOVER;
			}
				


		}
		else if (item->state != IDLE)  // MOUSE OUTSIDE HITBOX
		{
			if (item->focusable == true)
			{
				if (item->state == DRAG)          // keep it on drag state until key up
				{
					if(mouseState != KEY_UP)
						item->OnDrag(mousePos);
					else
					{
						item->state = IDLE;
						item->OnClickUp();
					}

				}
			}
			else
			{
				if (item->state == DRAG)
					item->SetOriginPos();

				item->state = IDLE;
				selectedItem = nullptr;
				item->OnHoverExit();
			}
		
		}
			
	 

	}

	lastMousePos = mousePos; 

	return true;
}




bool j1Gui::PostUpdate()
{

	BROFILER_CATEGORY("GUI PostUpdate", Profiler::Color::Gainsboro);
 
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)    
		debug = !debug;

	if(currentCanvas != nullptr)
		if(App->fade->GetCurrentStep() == fade_step::none)  // do not draw gui until fade is complete
			currentCanvas->DrawChildren();



	return true;
}

bool j1Gui::CleanUp()
{
	if (atlas != nullptr)
	{
		App->tex->UnLoad(atlas);
		atlas = nullptr;
	}

	for (auto& item : listOfItems)    
	{
		if (item != nullptr)
		{
			item->CleanUp();
			delete item;
			item = nullptr;
		}
	}
	listOfItems.clear();

	selectedItem = nullptr; 

	return true;
}

void j1Gui::LoadXMLGUI(pugi::xml_node& menuNode)
{

	// images 
	for (auto uiNode = menuNode.child("images").child("image"); uiNode; uiNode = uiNode.next_sibling("image"))
	{
		std::string name = uiNode.attribute("name").as_string();
		SDL_Rect section = { uiNode.child("section").attribute("x").as_int(), uiNode.child("section").attribute("y").as_int(), uiNode.child("section").attribute("w").as_int(), uiNode.child("section").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		std::string textureName = uiNode.child("textureMapName").attribute("value").as_string();
		float scaleFactor = uiNode.child("scaleFactor").attribute("value").as_float();

	    App->gui->AddImage(position, &section, name, NULL, scaleFactor, textureName);
	}


	// labels
	for (auto uiNode = menuNode.child("labels").child("label"); uiNode; uiNode = uiNode.next_sibling("label"))
	{
		std::string name = uiNode.attribute("name").as_string();
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		std::string text = uiNode.child("text").attribute("value").as_string();
		std::string font = uiNode.child("font").attribute("value").as_string();
		SDL_Color color = { uiNode.child("color").attribute("R").as_uint(),uiNode.child("color").attribute("G").as_uint(),uiNode.child("color").attribute("B").as_uint(),uiNode.child("color").attribute("A").as_uint() };
		const char* path = uiNode.child("path").attribute("p").as_string();
		uint size = uiNode.child("size").attribute("s").as_int();
		float scaleFactor = uiNode.child("scaleFactor").attribute("value").as_float(); 
		bool resetable = uiNode.child("resetable").attribute("value").as_bool();
		App->gui->AddLabel(name, text.data(), color, App->font->Load(path, size), position, NULL, scaleFactor, resetable);

	}

	

}


UiItem_Label * j1Gui::AddLabel(std::string name, std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem * const parent, float SpriteScale, bool resetable)
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = DBG_NEW UiItem_Label(name, text, color, font, position, currentCanvas, SpriteScale, resetable);
	else
		newUIItem = DBG_NEW UiItem_Label(name, text, color, font, position, parent, SpriteScale, resetable);

	listOfItems.push_back(newUIItem);
	return (UiItem_Label*)newUIItem;
}

UiItem_Image* j1Gui::AddImage(iPoint position, const SDL_Rect* section, std::string name, UiItem* const parent, float spriteScale, std::string newTextureName)
{
	UiItem* newUIItem = nullptr;

	if (parent == NULL)
		newUIItem = DBG_NEW UiItem_Image(position, section, name, currentCanvas, spriteScale, newTextureName);
	else
		newUIItem = DBG_NEW UiItem_Image(position, section, name, parent, spriteScale);

	listOfItems.push_back(newUIItem);

	return (UiItem_Image*)newUIItem;
}

UiItem* j1Gui::GetCanvasItemByName(std::string name, UiItem* parent) const  // searches item only in the current canvas childhood 
{
	if (parent == nullptr)   // if parent is not defined, start searching from canvas 
		parent = currentCanvas;

	for (const auto& item : parent->children)
	{
		if (item->name == name)  // if found, return 
			return item;
		else
			if (item->children.size() > 0)   // do a recursive call with the child's children
			{
				UiItem* success = GetCanvasItemByName(name, item);

				if (success != nullptr)
					return success;
			}
	}


	return nullptr;
	
}
