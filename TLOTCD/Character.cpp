#include "Character.h"
#include "UiItem_Label.h"
#include "UiItem_Image.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1EntityFactory.h"
#include "RNG.h"

Character::Character(bool active, bool enemy, bool AI, UiItem_Image* icon, Stats stats) : active(active), enemy(enemy), AI(AI), icon(icon), stats(stats)
{
	hpLabel = App->gui->AddLabel("HP", std::string("HP: " + std::to_string(stats.HP)), { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 60), nullptr, 1.F); 
	chargeLabel = App->gui->AddLabel("CHARGE", std::string("CHARGE: " + std::to_string(stats.charge)), { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 80), nullptr, 1.F);
	characterLabel = App->gui->AddLabel("CHAR", (enemy) ? "Character 2" : "Character 1", { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 40), nullptr, 1.F);

	infoLabels.push_back(App->gui->AddLabel("DMG", std::string("WEAK ATT: " + std::to_string(stats.dmg)), { 200, 200, 200, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 120), nullptr, 1.F));
	infoLabels.push_back(App->gui->AddLabel("DMG", std::string("STRONG ATT: " + std::to_string(stats.dmg2)), { 200, 200, 200, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 140), nullptr, 1.F));
	infoLabels.push_back(App->gui->AddLabel("DMG", std::string("WEAK ATT CHARGE: " + std::to_string(stats.dmgCharge)), { 200, 200, 200, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 160), nullptr, 1.F));
	infoLabels.push_back(App->gui->AddLabel("DMG", std::string("STRONG ATT CHARGE: " + std::to_string(stats.dmg2Charge)), { 200, 200, 200, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 180), nullptr, 1.F));
	infoLabels.push_back(App->gui->AddLabel("DMG", std::string("BLOCK ABSORTION: " + std::to_string((int)(stats.blockPercentatge * 100.f)) + "%"), { 200, 200, 200, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 200), nullptr, 1.F));
	infoLabels.push_back(App->gui->AddLabel("DMG", std::string("EVADE PROBABILITY: " + std::to_string(stats.evadeProb) + "%"), { 200, 200, 200, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 160, 220), nullptr, 1.F));

	if (active == false)
	{
		hpLabel->hide = chargeLabel->hide = characterLabel->hide = icon->hide = true;
		for (auto& l : infoLabels)
			l->hide = true; 
	}
		


	
}

Character::Character(bool active, bool enemy, bool AI, UiItem_Image* icon) : active(active), enemy(enemy), AI(AI), icon(icon)
{
	hpLabel = App->gui->AddLabel("HP", std::string("HP: " + std::to_string(stats.HP)), { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 50, -40), nullptr, 1.F);
	chargeLabel = App->gui->AddLabel("CHARGE", std::string("CHARGE: " + std::to_string(stats.charge)), { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 50, -20), nullptr, 1.F);
	characterLabel = App->gui->AddLabel("CHARGE", (enemy) ? "Character 2" : "Character 1", { 255, 255, 255, 255 }, App->font->defaultFont,
		icon->GetPos() + iPoint(icon->hitBox.w / 2 + 50, -60), nullptr, 1.F);

	if (active == false)
		hpLabel->hide = chargeLabel->hide = characterLabel->hide = icon->hide = true;
}


void Character::Update(float dt)
{
	if (actionCompleted == true)
		return; 

	if (AI == false)
	{
		if (attackTurn)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				if (stats.charge >= stats.dmgCharge)
				{
					Attack(stats.dmg, stats.dmgCharge);
					actionCompleted = true;
				}
	
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				if (stats.charge >= stats.dmg2Charge)
				{
					Attack(stats.dmg2, stats.dmg2Charge);
					actionCompleted = true;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				ReCharge(); 
				actionCompleted = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{

				if (stats.HP < stats.initialHP)
				{
					Heal(); 
					actionCompleted = true;
				}
			
			}

		}

		else
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				currentDefense = currenDefense::BLOCK;
				actionCompleted = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				currentDefense = currenDefense::EVADE;
				actionCompleted = true;
			}

			if (currentDefense == currenDefense::EVADE)
				Evade();
		}


	}
	else
	{
		if (stats.currentTime == 0.f) // first frame
		{
			// Make Decision
			if (this->incivible == false)
			{
				if (attackTurn)
				{
					switch (mode)
					{
					case strategyMode::RANDOM:
					{
						auto v = std::get<int>(App->entityFactory->rng->GetRandomValue(0, 3));
						switch (v)
						{
						case 0:
						{
							AI_WeakAttack();
							break;
						}
						case 1:
						{
							AI_StrongAttack();
							break;
						}
						case 2:
						{
							AI_Recharge();
							break;
						}
						case 3:
						{
							AI_Heal();
							break;
						}
						default:
							break;
						}
						break;
					}

					case strategyMode::WEAK_ATTACK:
					{
						AI_WeakAttack();

						break;
					}

					case strategyMode::STRONG_ATTACK:
					{
						AI_StrongAttack();

						break;
					}

					case strategyMode::CHARGE:
					{
						AI_Recharge();
						break;
					}

					case strategyMode::HP:
					{
						AI_Heal();
						break;
					}

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
						auto v = std::get<int>(App->entityFactory->rng->GetRandomValue(0, 1));
						if (v == 0)
							currentDefense = currenDefense::BLOCK;
						else
							currentDefense = currenDefense::EVADE;

						break;
					}

					case defenseStrategyMode::BLOCK:
						currentDefense = currenDefense::BLOCK;
						break;
					case defenseStrategyMode::EVADE:
						currentDefense = currenDefense::EVADE;
						break;
					default:
						break;
					}

					if (currentDefense == currenDefense::EVADE)
						Evade();

				}

			}
			else
				Invincible();
			
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

void Character::Invincible()
{
	Character* enemy = nullptr; 
	for(auto& c : App->entityFactory->characters)
		if (c->active && c != this) {
			enemy = c; 
			break; 
		}

	if (!enemy)
		return; 

	if (attackTurn)
	{
		// Finish the enemy with weak attack
		int enemyHP = enemy->stats.HP; 
		enemyHP -= (stats.dmg - stats.dmg * enemy->stats.blockPercentatge); 
		if (stats.charge >= stats.dmgCharge && enemyHP <= 0)
		{
			Attack(stats.dmg, stats.dmgCharge, false);
			return; 
		}

		// Finish the enemy with strong attack
		enemyHP = enemy->stats.HP;
		enemyHP -= (stats.dmg2 - stats.dmg2 * enemy->stats.blockPercentatge); 
		if (stats.charge >= stats.dmg2Charge && enemyHP <= 0)
		{
			Attack(stats.dmg2, stats.dmg2Charge, false);
			return;
		}
			
		// Otherwise
		// As a reference, always try to have more HP than the enemy's stronger attack
		if (stats.HP <= enemy->stats.dmg2)
			Heal(false); 
		else
		{
			if (stats.charge < stats.dmgCharge)
				ReCharge(false); 
			else
				AI_StrongAttack();
			
		}
	}
	else
	{
		// Block always, unless an enemy strong attack would kill me even if blocking, then evade
		int HP = stats.HP; 
		currentDefense = currenDefense::BLOCK;
		if ((HP -= (enemy->stats.dmg2 - stats.blockPercentatge * enemy->stats.dmg2)) <= 0)
			currentDefense = currenDefense::EVADE;
			
		if (currentDefense == currenDefense::EVADE)
			Evade();
	}

}

// Used in various situations
void Character::Attack(unsigned int damage, unsigned int damageCharge, bool updateLabel)
{
	App->entityFactory->currentAttack = damage; 
	stats.charge -= damageCharge; 

	if (updateLabel == false)
		return; 

	SDL_Color c = { 255, 255, 255, 255 };
	chargeLabel->ChangeTextureIdle(std::string("CHARGE: " + std::to_string(stats.charge)),
		&c, App->font->defaultFont);

}

void Character::ReCharge(bool updateLabel)
{
	stats.charge += RECHARGE; 
	lastPassiveAction = "The Attacker Recharged";

	if (updateLabel == false)
		return;

	SDL_Color c = { 255, 255, 255, 255 };
	chargeLabel->ChangeTextureIdle(std::string("CHARGE: " + std::to_string(stats.charge)),
		&c, App->font->defaultFont);


}

void Character::Heal(bool updateLabel)
{
	stats.HP += (unsigned int)(int)((float)(int)stats.initialHP * 0.1F);
	lastPassiveAction = "The Attacker Healed";

	if (stats.HP > stats.initialHP)
		stats.HP = stats.initialHP;

	if (updateLabel == false)
		return;

	SDL_Color c = { 255, 255, 255, 255 };
	hpLabel->ChangeTextureIdle(std::string("HP: " + std::to_string(stats.HP)),
		&c, App->font->defaultFont);


}

void Character::Evade()
{
	auto v = std::get<int>(App->entityFactory->rng->GetRandomValue(0, 100));
	if (v > stats.evadeProb)
		stats.lastEvaded = false;
	else
		stats.lastEvaded = true;
}

void Character::AI_WeakAttack()
{
	if (stats.charge >= stats.dmgCharge)
		Attack(stats.dmg, stats.dmgCharge, false);
	else  
		ReCharge(false);
}

void Character::AI_StrongAttack()
{
	if (stats.charge >= stats.dmg2Charge)
		Attack(stats.dmg2, stats.dmg2Charge, false);
	else  
		ReCharge(false);
}

void Character::AI_Recharge()
{
	if (stats.charge < stats.initialCharge)
		ReCharge(false);
	else
		AI_StrongAttack();
}

void Character::AI_Heal()
{
	if (stats.HP < stats.initialHP)
		Heal(false);
	else // if not damaged, proceed with weak attack logic
		AI_WeakAttack(); 
}

void Character::UpdateLabels()
{
	SDL_Color c = { 255, 255, 255, 255 };
	hpLabel->ChangeTextureIdle(std::string("HP: " + std::to_string(stats.HP)),
		&c, App->font->defaultFont);

	chargeLabel->ChangeTextureIdle(std::string("CHARGE: " + std::to_string(stats.charge)),
		&c, App->font->defaultFont);

}

void Character::Reset()
{
	lastPassiveAction = "empty"; 
	stats.Reset(); 
	actionCompleted = false; 
	attackTurn = false; 
	UpdateLabels();
}

void Character::Deactivate()
{
	lastPassiveAction = "empty";
	stats.Reset();
	actionCompleted = false;
	attackTurn = false;
	active = false; 
	hpLabel->hide = chargeLabel->hide = characterLabel->hide = icon->hide = true;
	for (auto& l : infoLabels)
		l->hide = true;
	UpdateLabels(); 
}

void Character::Activate()
{ 
	active = true; 
	hpLabel->hide = chargeLabel->hide = characterLabel->hide = icon->hide = false;
	for (auto& l : infoLabels)
		l->hide = false;
	UpdateLabels();
}

void Character::LevelUp()
{
	if (this->AI == true)
		return;

	static unsigned int times = 0; 

	if ((++times) > 3)
		return; 

	stats.Reset(); 
	stats.dmg += 5; 
	stats.dmg2 += 5; 
	stats.initialHP += 10; 
	stats.HP += 10; 
	UpdateLabels();

    // Update damage info labels
	SDL_Color c = { 255, 255, 255, 255 };
	infoLabels.at(0)->ChangeTextureIdle(std::string("WEAK ATT: " + std::to_string(stats.dmg)),
		&c, App->font->defaultFont);
	infoLabels.at(1)->ChangeTextureIdle(std::string("STRONG ATT: " + std::to_string(stats.dmg2)),
		&c, App->font->defaultFont);

}