#include "Character.h"
#include "UiItem_Label.h"
#include "UiItem_Image.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1EntityFactory.h"
#include "RNG.h"

Character::Character(bool active, bool enemy, bool AI, UiItem_Image* icon) : active(active), enemy(enemy), AI(AI), icon(icon)
{
	hpLabel = App->gui->AddLabel("HP", std::string("HP: " + std::to_string(stats.HP)), { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 50, -60), nullptr, 1.F); 
	chargeLabel = App->gui->AddLabel("CHARGE", std::string("CHARGE: " + std::to_string(stats.charge)), { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 50, -40), nullptr, 1.F);

}

void Character::Update(float dt)
{

	if (AI == false)
	{
		if (attackTurn)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{

				actionCompleted = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				actionCompleted = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				stats.charge += 2;
				actionCompleted = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				actionCompleted = true;
			}

		}

		else
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{

				actionCompleted = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				actionCompleted = true;
			}
		}


	}
	else
	{
		// Make Decision
		if (attackTurn)
		{
			switch (mode)
			{
			case strategyMode::RANDOM:
				break;
			case strategyMode::WEAK_ATTACK:
				break;
			case strategyMode::STRONG_ATTACK:
				break;
			case strategyMode::CHARGE:
				break;
			case strategyMode::HP:
				break;
			default:
				break;
			}
		}
		else 
		{
			switch (dMode)
			{
			case defenseStrategyMode::RANDOM:
			{

				break;
			}
				
			case defenseStrategyMode::BLOCK:
				break;
			case defenseStrategyMode::EVADE:
				break;
			default:
				break;
			}
		}


		// Await some time
		if ((stats.currentTime += dt) >= stats.thinkTime)
		{
			stats.currentTime = 0.f; 
			actionCompleted = true; 
		}
	}

	App->entityFactory->CheckBothCompleted(); 

}