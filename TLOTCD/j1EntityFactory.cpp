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
	UiItem_Image* img1 = App->gui->AddImage({ 100,330 }, &s, "Img", nullptr, 1.0F);
	Stats stats1;
	stats1.blockPercentatge = 0.5f;
	stats1.initialHP = stats1.HP = 70;
	stats1.dmg = 20;
	stats1.dmg2 = 30;
	stats1.dmgCharge = 2;
	stats1.dmg2Charge = 3;
	stats1.evadeProb = 35;
	auto character = DBG_NEW Character(true, false, false, img1, stats1);
	characters.push_back(character);
	characters.at(0)->attackTurn = true; 
	allyCharacters.push_back(character);

	s = { 0,852,290,388 };
	UiItem_Image* img2 = App->gui->AddImage({ 660,120 }, &s, "Img", nullptr, 1.0F);
	stats1.blockPercentatge = 0.2f;
	stats1.initialHP = stats1.HP = 110;
	stats1.dmg = 50;
	stats1.dmg2 = 60;
	stats1.dmgCharge = 5;
	stats1.dmg2Charge = 6;
	stats1.evadeProb = 10;
	auto character2 = DBG_NEW Character(true, true, true, img2, stats1);
	characters.push_back(character2);
	enemyCharacters.push_back(character2);

	s = { 0,1264,259,295 };
	UiItem_Image* img3 = App->gui->AddImage({ 660,120 }, &s, "Img", nullptr, 1.0F);
	stats1.blockPercentatge = 0.2f;
	stats1.initialHP = stats1.HP = 45;
	stats1.dmg = 10;
	stats1.dmg2 = 20;
	stats1.dmgCharge = 1;
	stats1.dmg2Charge = 2;
	stats1.evadeProb = 40;
	auto character3 = DBG_NEW Character(false, true, true, img3, stats1);
	characters.push_back(character3);
	enemyCharacters.push_back(character3);

	s = { 294,1231,250,368 };
	UiItem_Image* img4 = App->gui->AddImage({ 100,330 }, &s, "Img", nullptr, 1.0F);
	stats1.blockPercentatge = 0.4f;
	stats1.initialHP = stats1.HP = 80;
	stats1.dmg = 25;
	stats1.dmg2 = 35;
	stats1.dmgCharge = 3;
	stats1.dmg2Charge = 4;
	stats1.evadeProb = 30;
	auto character4 = DBG_NEW Character(false, false, true, img4, stats1);
	characters.push_back(character4);
	allyCharacters.push_back(character4);


	s = { 366,1636,197,320 };
	UiItem_Image* img5 = App->gui->AddImage({ 100,330 }, &s, "Img", nullptr, 1.0F);
	stats1.blockPercentatge = 0.4f;
	stats1.initialHP = stats1.HP = 60;
	stats1.dmg = 20;
	stats1.dmg2 = 30;
	stats1.dmgCharge = 2;
	stats1.dmg2Charge = 3;
	stats1.evadeProb = 40;
	auto character5 = DBG_NEW Character(false, false, true, img5, stats1);
	characters.push_back(character5);
	allyCharacters.push_back(character5);

	s = { 0,1598,281,357 };
	UiItem_Image* img6 = App->gui->AddImage({ 660,120 }, &s, "Img", nullptr, 1.0F);
	stats1.blockPercentatge = 0.25f;
	stats1.initialHP = stats1.HP = 95;
	stats1.dmg = 40;
	stats1.dmg2 = 50;
	stats1.dmgCharge = 4;
	stats1.dmg2Charge = 5;
	stats1.evadeProb = 15;
	auto character6 = DBG_NEW Character(false, true, true, img6, stats1);
	characters.push_back(character6);
	enemyCharacters.push_back(character6);


	// for the player
	s = { 572,919,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 100,20 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1000,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 100,100 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1077,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 100,180 }, &s, "Img", nullptr, 1.0F));
	s = { 572,1150,238,65 };
	actionHelpers.push_back(App->gui->AddImage({ 100,260 }, &s, "Img", nullptr, 1.0F));
	s = { 842,919,238,65 };
	defenseHelpers.push_back(App->gui->AddImage({ 100,20 }, &s, "Img", nullptr, 1.0F));
	s = { 842,1000,238,65 };
	defenseHelpers.push_back(App->gui->AddImage({ 100,100 }, &s, "Img", nullptr, 1.0F));

	// Core helpers
	dmgLabel = App->gui->AddLabel("dmgLabel", "Empty", { 255, 255, 255, 255 }, App->font->defaultFont, iPoint(450, 25), nullptr, 1.F);
	dmgLabel->hide = true; 
	winner = App->gui->AddLabel("winLabel", "Empty", { 255, 255, 255, 255 }, App->font->defaultFont, iPoint(450, 100), nullptr, 1.F);
	winner->hide = true;
	allyName = App->gui->AddLabel("CharLabel", "Character 1", { 0, 255, 0, 255 }, App->font->defaultFont, iPoint(450, 50), nullptr, 1.F);
	enemyName = App->gui->AddLabel("CharLabel", "Character 2", { 255, 0, 0, 255 }, App->font->defaultFont, iPoint(450, 75), nullptr, 1.F);

	// Toggle AI Modes
	AIHelpers.push_back(App->gui->AddLabel("h", "Random Strategy", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(50, 170), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Weak Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(50, 195), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Strong Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(50, 220), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Charge", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(50, 245), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Heal", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(50, 270), nullptr, 1.F));

	AIHelpers.push_back(App->gui->AddLabel("h", "Random Strategy", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(300, 170), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Block", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(300, 195), nullptr, 1.F));
	AIHelpers.push_back(App->gui->AddLabel("h", "Evade", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(300, 220), nullptr, 1.F));


	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Random Strategy", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 450), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Weak Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 475), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Strong Attack", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 500), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Charge", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 525), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Heal", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(800, 550), nullptr, 1.F));

	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Random Strategy", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(1050, 450), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Block", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(1050, 475), nullptr, 1.F));
	AIHelpersEmemy.push_back(App->gui->AddLabel("h", "Evade", { 255, 255, 255, 255 }, App->font->defaultFont,
		iPoint(1050, 500), nullptr, 1.F));

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

	if (winner->hide == false)
	{
		if ((popUpWinCurrentTime += App->GetDt()) >= popUpWinTime)
		{
			popUpWinCurrentTime = 0.f;
			winner->hide = true;
		}
	}

	// Level Up
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		allyCharacters.at(0)->LevelUp(); 



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
			damage =((float)currentAttack >= (float)lastDefender->stats.HP) 
			? (float)currentAttack : (float)currentAttack - ((float)currentAttack * (float)lastDefender->stats.blockPercentatge);
	
		else
		{
			if (lastDefender->stats.lastEvaded == false)
				damage = currentAttack; 
		}

		lastDefender->stats.HP -= (damage > (float)(int)lastDefender->stats.HP) ? 
			lastDefender->stats.HP : (unsigned int)(int)damage;
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

	// Update the labels in case it was AI
	if (lastAttacker->AI)
		lastAttacker->UpdateLabels(); 

	// Reset state
	lastDefender->actionCompleted = lastAttacker->actionCompleted = false;
	currentAttack = 0;

	// Death
	if (lastDefender->stats.HP <= 0)
		Death(lastDefender);
	else
	{
		// Defender now attacks
		lastDefender->attackTurn = true;

		// Attacker now defends
		lastAttacker->attackTurn = false;

	}

}

void j1EntityFactory::ToggleBattleMode()
{
	AIvsAI = !AIvsAI; 

	ResetAIHelperColors(true, true); 

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
	AIHelpersEmemy.at(static_cast<int>(enemyCharacters.at(i)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
	AIHelpersEmemy.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(enemyCharacters.at(i)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);


	// Turn color label
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
	allyName->ChangeTextureIdle("", &green, App->font->defaultFont);
	enemyName->ChangeTextureIdle("", &red, App->font->defaultFont);
}

void j1EntityFactory::Death(Character* dead)
{

	if (AIvsAI)
	{
		// Deactivate the AI
		dead->Deactivate();
		
		if(dead->enemy)
			ResetAIHelperColors(false, true);
		else
			ResetAIHelperColors(true, false);

		// Spawn new ally
		if (dead->enemy == false)
		{
			int i = std::get<int>(rng->GetRandomValue(1, (int)allyCharacters.size() - 1));
			allyCharacters.at(i)->Activate();

			// Helpers
			SDL_Color active = { 0, 255, 0, 255 };
			AIHelpers.at(static_cast<int>(allyCharacters.at(i)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
			AIHelpers.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(allyCharacters.at(i)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);

			// Reset Enemy
			for (auto& c : enemyCharacters)
				if (c->active && c->AI)
					c->Reset();

		}
		else 	// Spawn new enemy
		{
		
			int i = std::get<int>(rng->GetRandomValue(0, (int)enemyCharacters.size() - 1));
			enemyCharacters.at(i)->Activate();

			// Helpers
			SDL_Color active = { 0, 255, 0, 255 };
			AIHelpersEmemy.at(static_cast<int>(enemyCharacters.at(i)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
			AIHelpersEmemy.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(enemyCharacters.at(i)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);
		
			// Reset Ally
			for (auto& c : allyCharacters)
				if (c->active && c->AI)
					c->Reset(); 
		}

		// Restore attack turn in the ally
		for (auto& c : allyCharacters)
			if (c->active && c->AI)
				c->attackTurn = true; 

	}
	else
	{
		// Reset player (stats only)
		if (dead->AI == false)
			dead->Reset();
	
		// Deactivate enemy
		ResetAIHelperColors(false, true);
		for (auto& c : enemyCharacters)
			if (c->active)
				c->Deactivate(); 

		// Spawn new enemy
		int i = std::get<int>(rng->GetRandomValue(0, (int)enemyCharacters.size() - 1));
		enemyCharacters.at(i)->Activate();

		// Helpers
		SDL_Color active = { 0, 255, 0, 255 };
		AIHelpersEmemy.at(static_cast<int>(enemyCharacters.at(i)->mode))->ChangeTextureIdle("", &active, App->font->defaultFont);
		AIHelpersEmemy.at(static_cast<int>(strategyMode::MAX_MODES) + static_cast<int>(enemyCharacters.at(i)->dMode))->ChangeTextureIdle("", &active, App->font->defaultFont);

		// Restore attack turn at player
		allyCharacters.at(0)->attackTurn = true; 

	}
	

	// Turn color label
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
	allyName->ChangeTextureIdle("", &green, App->font->defaultFont);
	enemyName->ChangeTextureIdle("", &red, App->font->defaultFont);

	// Winner label
	winner->hide = false; 
	std::string text = ((dead->enemy) ? "CHARACTER 1 WINS" : "CHARACTER 2 WINS");
}

void j1EntityFactory::ResetAIHelperColors(bool ally, bool enemy)
{
	SDL_Color c = { 255, 255, 255, 255 };
	if (ally)
	{
		for (auto& h : AIHelpers)
			h->ChangeTextureIdle("", &c, App->font->defaultFont);
	}

	if (enemy)
	{
		for (auto& h : AIHelpersEmemy)
			h->ChangeTextureIdle("", &c, App->font->defaultFont);
	}
	
}