#include "Random.h"

Random::Random() : rnGen(seed()){
}

int Random::getIntInRange(int min, int max){
	return std::uniform_int_distribution<int>(min, max)(rnGen);
}

double Random::getDoubleInRange(double min, double max){
	return std::uniform_real_distribution<double>(min, max)(rnGen);
}