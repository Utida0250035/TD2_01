#pragma once

#include "DeathParticles.h"
#include "KamataEngine.h"
#include "Fade.h"
#include <memory>

class TitleScene {

public:

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut
	};

private:

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::WorldTransform playerWorldTransform_{};

	KamataEngine::Model* titleLogoModel_ = nullptr;
	KamataEngine::WorldTransform titleLogoWorldTransform_{};
	float currentT_ = 0.0f;
	float tSpeed_ = 1.0f;

	float deltaTime_ = 0.0f;

	bool isFinished_ = false;

	Phase phase_ = Phase::kFadeIn;

	std::unique_ptr<Fade> fade_ = nullptr;

	inline static constexpr float kFadeDuration = 1.0f;

public:

	void Initialize();
	void Update();
	void Draw();

	bool GetIsFinished() const { return isFinished_; };

	~TitleScene();

};