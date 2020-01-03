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
	SDL_Rect s = { 309, 852, 148, 355 };
	UiItem_Image* img1 = App->gui->AddImage({ 100,300 }, &s, "Img", nullptr, 1.0F);
	auto character = DBG_NEW Character(true, false, false, img1); 
	characters.push_back(character);
	characters.at(0)->attackTurn = true; 
	allyCharacters.push_back(character);


	s = { 0,852,290,388 };
	UiItem_Image* img2 = App->gui->AddImage({ 800,100 }, &s, "Img", nullptr, 1.0F);
	auto character2 = DBG_NEW Character(true, true, true, img2); 
	characters.push_back(character2);
	enemyCharacters.push_back(character2);

	s = { 0,1264,259,295 };
	UiItem_Image* img3 = App->gui->AddImage({ 800,100 }, &s, "Img", nullptr, 1.0F);
	auto character3 = DBG_NEW Character(false, true, true, img3); 
	characters.push_back(character3);
	enemyCharacters.push_back(character3);

	s = { 294,1231,250,368 };
	UiItem_Image* img4 = App->gui->AddImage({ 100,300 }, &s, "Img", nullptr, 1.0F);
	auto character4 = DBG_NEW Character(false, false, true, img4);
	characters.push_back(character4);
	allyCharacters.push_back(character4);

	// for the player
	s = { 572,919,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300,330 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1000,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300,410 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1077,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300,490 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1150,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 300, 570 }, &s, "Img", nullptr, 1.0F));
	s = { 842,919,238,65 };
	defenseHelpers.push_back(App->gui->AddImage({ 300,330 }, &s, "Img", nullptr, 1.0F));
	s = { 842,1000,238,65 };
	defenseHelpers.push_back(App->gui->AddImage({ 300,410 }, &s, "Img", nullptr, 1.0F));

	// Core helpers
	dmgLabel = App->gui->AddLabel("dmgLabel", "Empty", { 255, 255, 255, 255 }, App->font->defaultFont, iPoint(500, 100), nullptr, 1.F);
	dmgLabel->hide = true; 
	allyName = App->gui->AddLabel("CharLabel", "Character 1", { 0, 255, 0, 255 }, App->font->defaultFont, iPoint(500, 125), nullptr, 1.F);
	enemyName = App->gui->AddLabel("CharLabel", "Character 2", { 255, 0, 0, 255 }, App->font->defaultFont, iPoint(500, 150), nullptr, 1.F);

	// Toggle AI Modes
	AIHelpers.push_back(App->gui->AddLabel("h", "Random Strategy", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(10, 35), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Weak Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(10, 60), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Strong Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(10, 85), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Charge", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(10, 110), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Heal", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(10, 135), nullptr, 1.F));

	AIHelpers.push_back(App->gui->AddLabel("h", "Random Strategy", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(260, 35), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Block", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(260, 60), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Evade", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(260, 85), nullptr, 1.F));


	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Random Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 550), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Weak Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 575), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Strong Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 600), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Charge", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 625), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Heal", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 650), nullptr, 1.F));

	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Random Defence", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(1050, 550), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Block", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(1050, 575), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Evade", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(1050, 600), nullptr, 1.F));

	// set active ones
	SDL_Color active = { 0, 255, 0, 255 };
	if (characters.at(0)->AI == true)
	{
		AIHelpers.at(static_cast<int>(characters.at(0)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
		AIHelpers.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(characters.at(0)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);
	}

	AIHelpersEmemy.at(static_cast<int>(characters.at(1)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
	AIHelpersEmemy.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(characters.at(1)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);


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

	Character* active1 = nullptr; 
	Character* active2 = nullptr;
	bool AI1 = false; 
	bool AI2 = false; 

	for (auto& c : characters)
	{
		if (c->active)
		{
			c->Update(dt);

			if (c->enemy == false)
			{
				AI1 = c->AI;
				active1 = c; 
			}
				
			else
			{
				AI2 = c->AI;
				active2 = c; 
			}
			 
		}

	}

	ToggleAIModes(AI1, AI2, active1, active2); 
	ToggleUIVisibility(AI1, AI2, active1, active2);

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
		ToggleBattleMode(); 

	return true;
}

void j1EntityFactory::ToggleAIModes(bool ally, bool enemy, Character* allyC, Character* enemyC)
{
	if (AIHelpers.at(0)->hide == ally)
	{
		for (auto& h : AIHelpers)
			h->hide = !h->hide;

	}

	if (AIHelpersEmemy.at(0)->hide == enemy)
	{
		for (auto& h : AIHelpersEmemy)
			h->hide = !h->hide;

	}

	auto toggle = [&](Character* character, bool k1, bool k2, bool k3, bool k4)
	{
		// Go down one attack mode in the enum
		if (k1)
		{
			character->mode = ((static_cast<int>(character->mode) + 1) == static_cast<int>(strategyMode::MAX_MODES))
				? (static_cast<strategyMode>(0)) : static_cast<strategyMode>(static_cast<int>(character->mode) + 1);
		}

		// Go up one attack mode in the enum
		if (k2)
		{
			character->mode = ((static_cast<int>(character->mode) - 1) == -1)
				? (static_cast<strategyMode>(static_cast<int>(strategyMode::MAX_MODES) - 1)) : static_cast<strategyMode>(static_cast<int>(character->mode) - 1);
		}

		// Go down one defence mode in the enum
		if (k3)
		{
			character->dMode = ((static_cast<int>(character->dMode) + 1) == static_cast<int>(defenseStrategyMode::MAX_MODES))
				? (static_cast<defenseStrategyMode>(0)) : static_cast<defenseStrategyMode>(static_cast<int>(character->dMode) + 1);
		}

		// Go up one defence mode in the enum
		if (k4)
		{
			character->dMode = ((static_cast<int>(character->dMode) - 1) == -1)
				? (static_cast<defenseStrategyMode>(static_cast<int>(defenseStrategyMode::MAX_MODES) - 1)) : static_cast<defenseStrategyMode>(static_cast<int>(character->dMode) - 1);
		}

		// Finally hilight in green the label 
		std::vector < UiItem_Label*> target;
		target = (character->enemy) ? AIHelpersEmemy : AIHelpers; 
		

		if (k1 || k2)
		{

			SDL_Color c = { 255, 255, 255, 255 };
			SDL_Color active = { 0, 255, 0, 255 };
			for (int i = 0; i < static_cast<int>(strategyMode::MAX_MODES); ++i)
				target.at(i)->ChangeTextureIdle("", &c, App->font->defaultFont);
			target.at(static_cast<int>(character->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
		}

		if (k3 || k4)
		{
			SDL_Color c = { 255, 255, 255, 255 };
			SDL_Color active = { 0, 255, 0, 255 };
			for (int i = static_cast<int>(strategyMode::MAX_MODES); i < target.size(); ++i)
				target.at(i)->ChangeTextureIdle("", &c, App->font->defaultFont);
			target.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(character->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);

		}
			

	}; 

	// Input
	if (ally)
		toggle(allyC, App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN, App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN,
			App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN, App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN);
	
	if (enemy)
		toggle(enemyC, App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN, App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN,
			App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN, App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN);
	

}

void j1EntityFactory::ToggleUIVisibility(bool ally, bool enemy, Character* allyC, Character* enemyC)
{
	// Show / Hide AI helpers
	bool noPlayer = false; 
	if (allyC->AI && enemyC->AI)
		noPlayer = true;

	if (AIHelpers.at(0)->hide == noPlayer)
	{
		for (auto& h : AIHelpers)
			h->hide = !noPlayer;
	}

	// Show / Hide Player helper
	if (noPlayer == false)
	{
		if (allyC->attackTurn)
		{
			if (actionHelpers.at(0)->hide == true)
				for (auto& h : actionHelpers)
					h->hide = false;

			if (defenseHelpers.at(0)->hide == false)
				for (auto& h : defenseHelpers)
					h->hide = true;
		}
		else
		{

			if (actionHelpers.at(0)->hide == false)
				for (auto& h : actionHelpers)
					h->hide = true;

			if (defenseHelpers.at(0)->hide == true)
				for (auto& h : defenseHelpers)
					h->hide = false;
		}
	}
		
	else
	{
		if (actionHelpers.at(0)->hide == false)
			for (auto& h : actionHelpers)
				h->hide = true;

		if (defenseHelpers.at(0)->hide == false)
			for (auto& h : defenseHelpers)
				h->hide = true;
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

	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
    if(allyName->color.r == 255)
		allyName->ChangeTextureIdle("", &green, App->font->defaultFont);
	else
		allyName->ChangeTextureIdle("", &red, App->font->defaultFont);
	
	if (enemyName->color.r == 255)
		enemyName->ChangeTextureIdle("", &green, App->font->defaultFont);
	else
		enemyName->ChangeTextureIdle("", &red, App->font->defaultFont);

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

void j1EntityFactory::ToggleBattleMode()
{
	static bool AIvsAI = false; 
	AIvsAI = !AIvsAI; 

	if (AIvsAI)
	{
		for (auto& c : characters)
		{
			if (c->active)
			{
				 
				if (c->AI == false)
					c->Deactivate();
				else
					c->Reset(); 
			}
		}

		// Careful!! Player is at pos 1
		int i = std::get<int>(rng->GetRandomValue(1, (int)allyCharacters.size() - 1)); 
		allyCharacters.at(i)->Activate();

		// Helpers
		SDL_Color active = { 0, 255, 0, 255 };
		AIHelpers.at(static_cast<int>(allyCharacters.at(i)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
		AIHelpers.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(allyCharacters.at(i)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);

		// Attack turn
		allyCharacters.at(i)->attackTurn = true; 
	}
	else
	{
		for (auto& c : characters)
		{
			if (c->active)
			{
				if (c->enemy == false)
					c->Deactivate();	
				else
					c->Reset(); 
			}
		}

		// Pick the player
		allyCharacters.at(0)->Activate();
		allyCharacters.at(0)->attackTurn = true;
	}
	

	// In any case, make a new enemy spawn
	for (auto& c : characters)
	{
		if (c->active)
		{
			if (c->enemy == true)
			{
				c->Deactivate();
				break; 
			}
		}
	}

	int i = std::get<int>(rng->GetRandomValue(0, (int)enemyCharacters.size() - 1));
	enemyCharacters.at(i)->Activate();

	// Helpers
	SDL_Color active = { 0, 255, 0, 255 };
	AIHelpers.at(static_cast<int>(enemyCharacters.at(i)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
	AIHelpers.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(enemyCharacters.at(i)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);


	// Turn color label
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
	allyName->ChangeTextureIdle("", &green, App->font->defaultFont);
	enemyName->ChangeTextureIdle("", &red, App->font->defaultFont);
}