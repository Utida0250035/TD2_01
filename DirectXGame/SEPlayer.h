#pragma once
#include <unordered_map>
#include <string>

class SEPlayer {
private:
	static SEPlayer* instance_;

	// コンストラクタとデストラクタの隠蔽
	SEPlayer() = default;
	~SEPlayer() = default;
	// コピーコンストラクタと代入演算子を無効化
	SEPlayer(const SEPlayer&) = delete;
	SEPlayer& operator=(const SEPlayer&) = delete;


	std::unordered_map<size_t, bool> sounds;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static SEPlayer* GetInstance();

	void Update();

	void Play(const std::string& filePath);
};