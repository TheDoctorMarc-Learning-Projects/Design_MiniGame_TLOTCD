#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Brofiler/Brofiler.h"
#include <algorithm>
#include "UiItem_Label.h"

#include <assert.h>
#include "RNG.h"

j1EntityFactory::j1EntityFactory()
{
	name.assign("entities");
	rng = DBG_NEW RNG(); 

}

j1EntityFactory::~j1EntityFactory()
{

	
}

bool j1EntityFactory::Awake(pugi::xml_node & node)
{
bool ret = true;
return ret;
}

bool j1EntityFactory::Start()
{
	SDL_Rect s = { 0,852,290,388 }; 
	UiItem_Image* img1 = App->gui->AddImage({ 800,100 }, &s, "Img", nullptr, 1.0F); 
	characters.push_back(DBG_NEW Character(true, true, true, img1)); 
	s = { 309,852,148,355 };
	UiItem_Image* img2 = App->gui->AddImage({ 100,300 }, &s, "Img", nullptr, 1.0F);
	characters.push_back(DBG_NEW Character(true, false, false, img2));
	characters.at(1)->attackTurn = true; 

	// for the player
	s = { 572,919,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300,300 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1000,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300,400 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1077,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300,500 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1150,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300, 600 }, &s, "Img", nullptr, 1.0F));
	s = { 842,919,238,65 };
	defenseHelpers.push_back(App->gui->AddImage({ 300,300 }, &s, "Img", nullptr, 1.0F));
	s = { 842,1000,238,65 };
	defenseHelpers.push_back(App->gui->AddImage({ 300,400 }, &s, "Img", nullptr, 1.0F));

	// Attacks
	dmgLabel = App->gui->AddLabel("dmgLabel", "Empty", { 255, 255, 255, 255 }, App->font->defaultFont, iPoint(500, 100), nullptr, 1.F);
	dmgLabel->hide = true; 


	return true;
}

bool j1EntityFactory::PreUpdate()
{
	BROFILER_CATEGORY("Entities PreUpdate", Profiler::Color::LavenderBlush);

	if (App->pause == true)
		return true;

	if (dmgLabel->hide == false)
	{
		if ((popUpCurrentTime += App->GetDt()) >= popUpTime)
		{
			popUpCurrentTime = 0.f; 
			dmgLabel->hide = true; 
		}
	}


	bool ret = true;

	return true;
}

bool j1EntityFactory::Update(float dt)
{
	BROFILER_CATEGORY("Entities Update", Profiler::Color::Fuchsia);

	for (auto& c : characters)
	{
		uint AI = 0; 
		if (c->active)
		{
			AI++;
			c->Update(dt);

			PlayerHelperDecider(c); 
		}
			
		if (AI == 2)
		{
			if (actionHelpers[0]->hide == false)
			{
				for (auto& helper : actionHelpers)
					helper->hide = false;
			}

			if (defenseHelpers[0]->hide == false)
			{
				for (auto& helper : defenseHelpers)
					helper->hide = false;
			}
		}

	}
		
 
	return true;
}


void j1EntityFactory::PlayerHelperDecider(Character* c)
{
	if (c->AI == false)
	{
		if (c->attackTurn == true)
		{
			if (actionHelpers[0]->hide == true)
			{
				for (auto& helper : actionHelpers)
					helper->hide = false;
			}


			if (defenseHelpers[0]->hide == false)
			{
				for (auto& helper : defenseHelpers)
					helper->hide = true;
			}

		}
		else
		{
			if (defenseHelpers[0]->hide == true)
			{
				for (auto& helper : defenseHelpers)
					helper->hide = false;
			}

			if (actionHelpers[0]->hide == false)
			{
				for (auto& helper : actionHelpers)
					helper->hide = true;
			}
		}

	}
}

bool j1EntityFactory::PostUpdate()
{
	BROFILER_CATEGORY("Entities PostUpdate", Profiler::Color::BurlyWood);
     

	// TODO (UI): blit character icon vs character icon


	if (App->pause == true)
		return true;


				
	return true;
}

void j1EntityFactory::SwitchTurn(Character* lastAttacker, Character* lastDefender)
{
	// Apply damage and update GUI
	auto currentDefMode = lastDefender->currentDefense;
	std::string popUpText = std::string(); 
	float damage = 0.f;
	if (currentAttack > 0)
	{
		
		if (currentDefMode == currenDefense::BLOCK)
			damage = (float)currentAttack - ((float)currentAttack * (float)lastDefender->stats.blockPercentatge);
	
		else
		{
			if (lastDefender->stats.lastEvaded == false)
				damage = currentAttack; 
		}
		lastDefender->stats.HP -= damage;
	}

	if (lastAttacker->lastPassiveAction == "empty")
	{
		if (damage > 0.f)
			popUpText = "Defender Received " + std::to_string((int)damage) + " Damage Of " + std::to_string((int)currentAttack);
		else
		{
			if (currentAttack > 0.f)
				popUpText = "Defender Evaded The Attack";
			else
				popUpText = "No Attack Was Made";
		}
	}
	else
	{
		popUpText = lastAttacker->lastPassiveAction; 
		lastAttacker->lastPassiveAction = "empty"; 
	}

	SDL_Color c = { 255, 255, 255, 255 };
	dmgLabel->hide = false; 
	dmgLabel->ChangeTextureIdle(popUpText,
		&c, App->font->defaultFont);

	lastDefender->hpLabel->ChangeTextureIdle(std::string("HP: " + std::to_string(lastDefender->stats.HP)),
		&c, App->font->defaultFont);



	// Defender now attacks
	lastDefender->attackTurn = true;

	// Attacker now defends
	lastAttacker->attackTurn = false;

	// Update the labels in case it was AI
	if (lastAttacker->AI)
		lastAttacker->UpdateLabels(); 

	// Reset state
	lastDefender->actionCompleted = lastAttacker->actionCompleted = false;
	currentAttack = 0;
}
