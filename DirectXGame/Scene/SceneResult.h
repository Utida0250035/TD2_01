#pragma once

#include "Scene.h"
#include <cstdint>
#include "../Audio/Audio.h"

#include "../Cloud.h"
#include "../Skydome.h"

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