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

	int32_t choice_ = 0;

	static inline constexpr int32_t kChoiceMax = 2;

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