#pragma once
#include <random>


namespace Utilities
{
	inline float GenerateRandomFloat(float min, float max)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dis(min, max);
		return dis(mt);
	}
}
