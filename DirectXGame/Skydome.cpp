#include "Skydome.h"

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();

}

void Skydome::Update() {}

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_); }