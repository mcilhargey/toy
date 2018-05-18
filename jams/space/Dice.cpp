
#include <space/Dice.h>

#include <chrono>

namespace mud
{
	std::default_random_engine Dice100::sGenerator = std::default_random_engine();
	std::uniform_int_distribution<int> Dice100::sDistribution = std::uniform_int_distribution<int>(1,100);
	
	void Dice100::seed()
	{
		std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::duration d;

		sGenerator.seed();
	}

	int Dice100::roll()
	{
		return sDistribution(sGenerator);
	}

	std::default_random_engine Dice10::sGenerator = std::default_random_engine();
	std::uniform_int_distribution<int> Dice10::sDistribution = std::uniform_int_distribution<int>(1,10);
	
	void Dice10::seed()
	{
		std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
		sGenerator.seed();
	}

	int Dice10::roll()
	{
		return sDistribution(sGenerator);
	}
}