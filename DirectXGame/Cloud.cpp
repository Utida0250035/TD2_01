#include "Cloud.h"
#include "../KeMatrix3D.h"
#include "Random.h"

void Cloud::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	model_ = model;
	camera_ = camera;
	color_.Initialize();
	color_.SetColor({1.0f, 1.0f, 1.0f, 0.3f});
	for (int i = 0; i < kCloudNum_; ++i) {
		worldTransforms_[i].Initialize();

		InitializeCloud(worldTransforms_[i]);
		worldTransforms_[i].translation_.x = Random::Create(-145.0f, 145.0f);

		UpdateWorldTransform(worldTransforms_[i]);
	}
}

void Cloud::Update() { 
	for (int i = 0; i < kCloudNum_; ++i) {
		if (worldTransforms_[i].translation_.x < -145.0f) {
			InitializeCloud(worldTransforms_[i]);
		}

		worldTransforms_[i].translation_.x -= kCloudspeed_;

		UpdateWorldTransform(worldTransforms_[i]);
	}
}

void Cloud::Draw() { 
	for (int i = 0; i < kCloudNum_; ++i) {
		model_->Draw(worldTransforms_[i], *camera_, &color_);
	}
}

void Cloud::InitializeCloud(KamataEngine::WorldTransform& worldTransform) {
	worldTransform.translation_.x = 145.0f;
	worldTransform.translation_.y = Random::Create(-80.0f, 80.0f);
	worldTransform.translation_.z = Random::Create(80.0f, 120.0f);

	worldTransform.rotation_.y = Random::Create(0.0f, 6.28f);
}