#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class SEPlayer {
private:
	static std::unique_ptr<SEPlayer> instance_;

	// コンストラクタとデストラクタの隠蔽
	SEPlayer() = default;

public:
	~SEPlayer() = default;

private:
	
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