#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
 
#include "SDL/include/SDL.h"
#include <vector>
#include <list>
#include "Color.h"
#include <map>
#include <array>
#include "p2Point.h"

class j1EntityFactory : public j1Module
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() { return true;  };

	bool hasIntersectionRectAndLine(const SDL_Rect* rect, std::array<int, 4> line) const  // line is passed like this: {x1, y1, x2, y2}
	{ 
		return SDL_IntersectRectAndLine(rect, &line[0], &line[1], &line[2], &line[3]);
	}

	bool isPointInsideRect(const iPoint* p, const SDL_Rect* r) const {
		const SDL_Point P = { p->x, p->y }; 
		return SDL_PointInRect(&P, r);
	}

public:

};




#endif  