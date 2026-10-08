#pragma once

#include <KamataEngine.h>
#include "Scene.h"

namespace Atrum {

class Entity;

class SceneTitle final : public Scene {
private:
	// タイトルロゴ
	std::unique_ptr<KamataEngine::Sprite> spriteTitle_;
	std::unique_ptr<KamataEngine::Sprite> spriteButton_;

	KamataEngine::Vector3 buttonTranslate_ = {0.0f, 0.0f, 0.0f};
	KamataEngine::Vector4 buttonColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
	bool isToAlpha_ = true;

public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneTitle* GetInstance() {

		static SceneTitle instance;

		return &instance;
	}
};

} // namespace Atrum