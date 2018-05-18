
#pragma once

#include <space/Generated/Module.h>

/* std */
#include <random>
#include <iostream>

namespace mud
{
	class _EX_SPACE_EXPORT Dice100
	{
	public:
		static std::default_random_engine sGenerator;
		static std::uniform_int_distribution<int> sDistribution;

		static void seed();
		static int roll();
	};

	class _EX_SPACE_EXPORT Dice10
	{
	public:
		static std::default_random_engine sGenerator;
		static std::uniform_int_distribution<int> sDistribution;

		static void seed();
		static int roll();
	};

	inline int random100(int base)
	{
		return int(std::ceil(base * Dice100::roll() / 100.f));
	}

	inline int fuzzy100(int base, int variance)
	{
		return int(base + (variance * (Dice100::roll() - 50) / 100));
	}
}
