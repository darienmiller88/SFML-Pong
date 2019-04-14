#pragma once
#include <random>

class Random{
	public:
		Random();
		int getIntInRange(int min, int max);
		double geDoubleInRange(double min, double max);
	private:
		std::random_device seed;
		std::default_random_engine rnGen;
};