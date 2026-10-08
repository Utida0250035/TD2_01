#include "Random.h"

// 乱数生成エンジン
std::random_device Random::seedGenerator;

// メルセンヌ・ツイスターエンジン
std::mt19937_64 Random::randomEngine;

int Random::Create(const int min, const int max) {

	randomEngine.seed(seedGenerator());

	std::uniform_int_distribution<int> rotationDistribution(min, max);

	return rotationDistribution(randomEngine);
}

float Random::Create(const float min, const float max) {

	randomEngine.seed(seedGenerator());

	std::uniform_real_distribution<float> rotationDistribution(min, max);

	return rotationDistribution(randomEngine);
}

uint32_t Random::Create(const uint32_t min, const uint32_t max) {

	randomEngine.seed(seedGenerator());

	std::uniform_int_distribution<uint32_t> rotationDistribution(min, max);

	return rotationDistribution(randomEngine);
}