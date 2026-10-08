#pragma once

#include <random>

// 乱数
class Random final {

private:
	// 乱数生成エンジン
	static std::random_device seedGenerator;

	// メルセンヌ・ツイスターエンジン
	static std::mt19937_64 randomEngine;

public:
	/// <summary>
	/// int型乱数生成
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	static int Create(const int min, const int max);

	/// <summary>
	/// float型乱数生成
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	static float Create(const float min, const float max);

	/// <summary>
	/// uint32_t型乱数生成
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	static uint32_t Create(const uint32_t min, const uint32_t max);
};