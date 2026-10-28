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

	// ランキング
	timeAttackManager_ = new TimeAttackManager;
	rankingLists_ = timeAttackManager_->GetRankingList();

	textureNums_[0] = KamataEngine::TextureManager::Load("0.png");
	textureNums_[1] = KamataEngine::TextureManager::Load("1.png");
	textureNums_[2] = KamataEngine::TextureManager::Load("2.png");
	textureNums_[3] = KamataEngine::TextureManager::Load("3.png");
	textureNums_[4] = KamataEngine::TextureManager::Load("4.png");
	textureNums_[5] = KamataEngine::TextureManager::Load("5.png");
	textureNums_[6] = KamataEngine::TextureManager::Load("6.png");
	textureNums_[7] = KamataEngine::TextureManager::Load("7.png");
	textureNums_[8] = KamataEngine::TextureManager::Load("8.png");
	textureNums_[9] = KamataEngine::TextureManager::Load("9.png");

	numbers_[0] = KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f});
	numbers_[1] = KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f});
	numbers_[2] = KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f});
	numbers_[3] = KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f});
	numbers_[4] = KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f});
	numbers_[5] = KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f});

	spriteColon_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("colon.png"), {-100.0f, -100.0f});
	spriteDot_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("dot.png"), {-100.0f, -100.0f});

	bgmHandle_ = Audio::Manager::GetInstance()->Play("./Resources/bgmResult.mp3", true);
}

void SceneResult::Update() {
	XINPUT_STATE state{};
	XINPUT_STATE preState{};

	if (KamataEngine::Input::GetInstance()->GetJoystickState(0, state)) {
		KamataEngine::Input::GetInstance()->GetJoystickStatePrevious(0, preState);
	}

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE) || (state.Gamepad.wButtons == XINPUT_GAMEPAD_A && preState.Gamepad.wButtons != XINPUT_GAMEPAD_A)) {
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

	int32_t rankingNum = 0;

	//for (auto& ranking : rankingLists_) {
		//ranking;
		time_ = SplitTime::FromSeconds(900.28f);

		timeCount_ = time_.minutes_;
		minutesNum_[0] = timeCount_ / 10;
		timeCount_ %= 10;
		minutesNum_[1] = timeCount_ / 1;

		timeCount_ = time_.seconds_;
		secondsNum_[0] = timeCount_ / 10;
		timeCount_ %= 10;
		secondsNum_[1] = timeCount_ / 1;

		timeCount_ = time_.centiseconds_;
		centiSecondsNum_[0] = timeCount_ / 10;
		timeCount_ %= 10;
		centiSecondsNum_[1] = timeCount_ / 1;

		for (int i = 0; i < 2; i++) {
		    numbers_[i]->SetTextureHandle(textureNums_[minutesNum_[i]]);
			numbers_[i]->SetPosition({i * 24.0f, 40.0f * rankingNum});
			numbers_[i]->Draw();
		}

		for (int i = 0; i < 2; i++) {
		    numbers_[i + 2]->SetTextureHandle(textureNums_[secondsNum_[i]]);
		    numbers_[i + 2]->SetPosition({(i + 3) * 24.0f, 40.0f * rankingNum});
		    numbers_[i + 2]->Draw();
		}

		for (int i = 0; i < 2; i++) {
		    numbers_[i + 4]->SetTextureHandle(textureNums_[centiSecondsNum_[i]]);
		    numbers_[i + 4]->SetPosition({(i + 6) * 24.0f, 40.0f * rankingNum});
		    numbers_[i + 4]->Draw();
		}

		spriteColon_->SetPosition({2 * 24.0f, 40.0f * rankingNum});
	    spriteColon_->Draw();

	    spriteDot_->SetPosition({5 * 24.0f, 40.0f * rankingNum});
	    spriteDot_->Draw();

		rankingNum++;
	//}

	KamataEngine::Sprite::PostDraw();
}

void SceneResult::ExitScene() {

	Audio::Manager::GetInstance()->Stop(bgmHandle_);

	delete skydome_;
	delete cloud_;
}

} // namespace Atrum