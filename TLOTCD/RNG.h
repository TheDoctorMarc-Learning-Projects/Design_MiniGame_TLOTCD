#pragma once
#include <variant>
#include <vector>
#include "pcg/include/pcg_random.hpp"

class RNG
{
public:
	bool Initialize(); 
	
public:
	 static std::variant<int, float> GetRandomValue(std::variant<int, float> start = 0,
		std::variant<int, float> end = INT_MAX);
private: 
	static pcg32 rng;
}; 
