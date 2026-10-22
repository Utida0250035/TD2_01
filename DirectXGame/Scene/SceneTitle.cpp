#include "../Scene/SceneTitle.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneGame.h"
#include <iostream>

#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"

namespace Atrum {

void SceneTitle::EnterScene() {

	std::cout << "TitleScene\n" << std::endl;
	std::cout << "key Space to go Select\n" << std::endl;

	// タイトルロゴの初期化
	uint32_t texture = KamataEngine::TextureManager::Load("titleLogo.png");
	spriteTitle_.reset(KamataEngine::Sprite::Create(texture, {640.0f, 300.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	uint32_t textureButton = KamataEngine::TextureManager::Load("navigationSpaceKey.png");
	spriteButton_.reset(KamataEngine::Sprite::Create(textureButton, {640.0f, 650.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	//-------------------
	// 背景

	skydome_ = new Skydome;
	skydome_->Initialize(KamataEngine::Model::CreateFromOBJ("skyDome", true), KeMainCamera::GetInstance()->camera_);

	cloud_ = new Cloud;
	cloud_->Initialize(KamataEngine::Model::CreateFromOBJ("cloud"), KeMainCamera::GetInstance()->camera_);

	//
	//-------------------

	bgmHandle_ = Audio::Manager::GetInstance()->Play("./Resources/bgmTitle.mp3", true);
}

void SceneTitle::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		CommandChangeScene::GetInstance()->Set(SceneGame::GetInstance(), new ChangeSceneFade());
	}

	// 透明度
	if (isToAlpha_) {
		buttonColor_.w -= 0.7f / 90.0f;
		if (buttonColor_.w <= 0.3f) {
			buttonColor_.w = 0.3f;
			isToAlpha_ = false;
		}
	} else {
		buttonColor_.w += 0.7f / 90.0f;
		if (buttonColor_.w >= 1.0f) {
			buttonColor_.w = 1.0f;
			isToAlpha_ = true;
		}
	}

	skydome_->Update();
	cloud_->Update();

	spriteButton_->SetColor(buttonColor_);
}

void SceneTitle::Draw() {

	KamataEngine::Model::PreDraw();

	skydome_->Draw();
	cloud_->Draw();

	KamataEngine::Model::PostDraw();

	KamataEngine::Sprite::PreDraw();

	spriteTitle_->Draw();

	spriteButton_->Draw();

	KamataEngine::Sprite::PostDraw();
}

void SceneTitle::ExitScene() {

	Audio::Manager::GetInstance()->Stop(bgmHandle_);

	delete skydome_;
	delete cloud_;

}

} // namespace Atrum