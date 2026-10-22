#include "../Scene/SceneResult.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneGame.h"
#include "../Scene/SceneSelect.h"
#include "../Scene/SceneTitle.h"
#include <KamataEngine.h>
#include <iostream>

#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"

namespace Atrum {

void SceneResult::EnterScene() {

	std::cout << "key A,D to select destination\n" << std::endl;

	std::cout << "key Space to go nextLevel\n" << std::endl;

	uint32_t texture = KamataEngine::TextureManager::Load("resultLogo.png");
	spriteResult_.reset(KamataEngine::Sprite::Create(texture, {640.0f, 150.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	textureKeyboard_ = KamataEngine::TextureManager::Load("navigationSpaceKey.png");
	textureController_ = KamataEngine::TextureManager::Load("navigationA.png");
	if (KamataEngine::Input::GetInstance()->GetNumberOfJoysticks()) {
		spriteButton_.reset(KamataEngine::Sprite::Create(textureController_, {640.0f, 650.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	} else {
		spriteButton_.reset(KamataEngine::Sprite::Create(textureKeyboard_, {640.0f, 650.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	}

	//-------------------
	// 背景

	skydome_ = new Skydome;
	skydome_->Initialize(KamataEngine::Model::CreateFromOBJ("skyDome", true), KeMainCamera::GetInstance()->camera_);

	cloud_ = new Cloud;
	cloud_->Initialize(KamataEngine::Model::CreateFromOBJ("cloud"), KeMainCamera::GetInstance()->camera_);

	//
	//-------------------

	bgmHandle_ = Audio::Manager::GetInstance()->Play("./Resources/bgmResult.mp3", true);
}

void SceneResult::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// タイトルへ

		// SceneTitleへの遷移
		CommandChangeScene::GetInstance()->Set(SceneTitle::GetInstance(), new ChangeSceneFade());
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

	if (KamataEngine::Input::GetInstance()->GetNumberOfJoysticks()) {
		spriteButton_->SetTextureHandle(textureController_);
	} else {
		spriteButton_->SetTextureHandle(textureKeyboard_);
	}
}

void SceneResult::Draw() {

	KamataEngine::Model::PreDraw();

	skydome_->Draw();
	cloud_->Draw();

	KamataEngine::Model::PostDraw();

	KamataEngine::Sprite::PreDraw();

	spriteResult_->Draw();

	spriteButton_->Draw();

	KamataEngine::Sprite::PostDraw();
}

void SceneResult::ExitScene() {

	Audio::Manager::GetInstance()->Stop(bgmHandle_);

	delete skydome_;
	delete cloud_;
}

} // namespace Atrum