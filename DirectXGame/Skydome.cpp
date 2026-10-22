#include "Skydome.h"

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();

	objectColor_.Initialize();
	objectColor_.SetColor({0.5f, 0.5f, 0.5f, 1.0f});

}

void Skydome::Update() {}

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_, &objectColor_); }