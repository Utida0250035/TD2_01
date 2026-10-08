#include "../Scene/SceneTitle.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneSelect.h"
#include <iostream>

namespace Atrum {

void SceneTitle::EnterScene() {

	std::cout << "TitleScene\n" << std::endl;
	std::cout << "key Space to go Select\n" << std::endl;

	// タイトルロゴの初期化
	uint32_t texture = KamataEngine::TextureManager::Load("white1x1.png");
	spriteTitle_.reset(KamataEngine::Sprite::Create(texture, {640.0f, 150.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	uint32_t textureButton = KamataEngine::TextureManager::Load("white1x1.png");
	spriteButton_.reset(KamataEngine::Sprite::Create(textureButton, {640.0f, 650.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	spriteTitle_->SetSize({400.0f, 100.0f});
	spriteButton_->SetSize({200.0f, 80.0f});
}

void SceneTitle::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		CommandChangeScene::GetInstance()->Set(SceneSelect::GetInstance());
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

	spriteButton_->SetColor(buttonColor_);
}

void SceneTitle::Draw() {
	KamataEngine::Sprite::PreDraw();

	spriteTitle_->Draw();

	spriteButton_->Draw();

	KamataEngine::Sprite::PostDraw();
}

void SceneTitle::ExitScene() {}

} // namespace Atrum