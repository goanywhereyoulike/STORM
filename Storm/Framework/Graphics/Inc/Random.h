#pragma once
#include "Common.h"
class Random
{

public:
	static void init()
	{
		RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)Distribution(RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> Distribution;

};