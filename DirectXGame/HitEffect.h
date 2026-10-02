#pragma once

#include "../Math/Vector3.h"
#include <KamataEngine.h>

class HitEffect {

private:
	static KamataEngine::Model* model_;
	static KamataEngine::Camera* camera_;

	Atrum::Math::Vector3 position_{};

	KamataEngine::WorldTransform worldTransforms_[3]{};
	KamataEngine::ObjectColor objectColor_{};

	float lifeTime_ = 0.0f;
	float lifeCount_ = 0.0f;

	float slashAngle_[2]{};

	bool isFinish_ = false;

public:

	static HitEffect* Create(const Atrum::Math::Vector3& position, const float lifeTime);

	void Initialize(const Atrum::Math::Vector3& position, const float lifeTime);

	void Update(const float deltaTime);

	void Draw();

	bool GetIsFinish() { return isFinish_; }

	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

};