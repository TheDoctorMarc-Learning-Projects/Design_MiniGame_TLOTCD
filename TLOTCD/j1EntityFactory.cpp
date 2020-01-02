#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Input.h"

#include "Brofiler/Brofiler.h"

#include <algorithm>

#include <assert.h>

j1EntityFactory::j1EntityFactory()
{
	name.assign("entities");

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

	

	return true;
}

bool j1EntityFactory::PreUpdate()
{
	BROFILER_CATEGORY("Entities PreUpdate", Profiler::Color::LavenderBlush);

	if (App->pause == true)
		return true;

	bool ret = true;

	return true;
}

bool j1EntityFactory::Update(float dt)
{
	BROFILER_CATEGORY("Entities Update", Profiler::Color::Fuchsia);

	if (App->pause == true)
		return true; 

	return true;
}

bool j1EntityFactory::PostUpdate()
{
	BROFILER_CATEGORY("Entities PostUpdate", Profiler::Color::BurlyWood);
     

	// TODO (UI): blit character icon vs character icon


	if (App->pause == true)
		return true;


				
	return true;
}
