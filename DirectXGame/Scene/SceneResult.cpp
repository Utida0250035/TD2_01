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
	choice_ = 0;

	std::cout << "key A,D to select destination\n" << std::endl;

	std::cout << "key Space to go nextLevel\n" << std::endl;

	uint32_t texture = KamataEngine::TextureManager::Load("resultLogo.png");
	spriteResult_.reset(KamataEngine::Sprite::Create(texture, {640.0f, 150.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	uint32_t textureButton = KamataEngine::TextureManager::Load("navigationSpaceKey.png");
	spriteButton_.reset(KamataEngine::Sprite::Create(textureButton, {640.0f, 650.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

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

	skydome_->Update();
	cloud_->Update();
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