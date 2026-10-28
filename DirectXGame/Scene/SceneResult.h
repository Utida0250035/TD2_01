#pragma once

#include "Scene.h"
#include <cstdint>
#include "../Audio/Audio.h"

#include "../Cloud.h"
#include "../Skydome.h"

#include "../Time/TimeAttackManager.h"

namespace Atrum {

class Entity;

class SceneResult final : public Scene {

private:

	static inline constexpr int32_t kChoiceMax = 2;

	std::unique_ptr<KamataEngine::Sprite> spriteResult_;
	std::unique_ptr<KamataEngine::Sprite> spriteButton_;

	uint32_t textureKeyboard_;
	uint32_t textureController_;

	KamataEngine::Vector3 buttonTranslate_ = {0.0f, 0.0f, 0.0f};
	KamataEngine::Vector4 buttonColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
	bool isToAlpha_ = true;

	//---------------------
	// 背景

	// 天球
	Skydome* skydome_ = nullptr;

	// 雲
	Cloud* cloud_ = nullptr;

	//
	//---------------------

	Audio::AudioHandle bgmHandle_;

	TimeAttackManager* timeAttackManager_ = nullptr;
	std::vector<float> rankingLists_{};
	KamataEngine::Sprite* numbers_[6];
	KamataEngine::Sprite* spriteColon_;
	KamataEngine::Sprite* spriteDot_;
	int32_t timeCount_;
	int32_t minutesNum_[2] = {0, 0};
	int32_t secondsNum_[2] = {0, 0};
	int32_t centiSecondsNum_[2] = {0, 0};
	SplitTime time_;
	uint32_t textureNums_[10];

public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneResult* GetInstance() {

		static SceneResult instance;

		return &instance;
	}
};

} // namespace Atrum