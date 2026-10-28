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
	timeAttackManager_ = SceneGame::GetInstance()->GetRtaManager();
	rankingLists_ = timeAttackManager_->GetRankingList();

	if (rankingLists_.size() < 3) {
		for (size_t i = 3; i > rankingLists_.size() - 1; --i) {
			rankingLists_.push_back(5999.99f);
		}
	}

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

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 3; ++j) {
			numbers_[i][j].reset(KamataEngine::Sprite::Create(textureNums_[0], {-100.0f, -100.0f}));
		}
	}

	for (int i = 0; i < 3; ++i) {
		spriteColon_[i].reset(KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("colon.png"), {-100.0f, -100.0f}));
		spriteDot_[i].reset(KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("dot.png"), {-100.0f, -100.0f}));
	}

	spriteRankingNum_[0].reset(KamataEngine::Sprite::Create(textureNums_[1], {-100.0f, -100.0f}, {1.0f, 1.0f, 0.1f, 1.0f}));
	spriteRankingDot_[0].reset(KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("dot.png"), {-100.0f, -100.0f}, {1.0f, 1.0f, 0.1f, 1.0f}));
	
	spriteRankingNum_[1].reset(KamataEngine::Sprite::Create(textureNums_[2], {-100.0f, -100.0f}, {0.5f, 0.5f, 0.5f, 1.0f}));
	spriteRankingDot_[1].reset(KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("dot.png"), {-100.0f, -100.0f}, {0.5f, 0.5f, 0.5f, 1.0f}));

	spriteRankingNum_[2].reset(KamataEngine::Sprite::Create(textureNums_[3], {-100.0f, -100.0f}, {0.7f, 0.3f, 0.15f, 1.0f}));
	spriteRankingDot_[2].reset(KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("dot.png"), {-100.0f, -100.0f}, {0.7f, 0.3f, 0.15f, 1.0f}));


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

	for (auto& ranking : rankingLists_) {
		time_ = SplitTime::FromSeconds(ranking);

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
		    numbers_[i][rankingNum]->SetTextureHandle(textureNums_[minutesNum_[i]]);
			numbers_[i][rankingNum]->SetPosition({i * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
			numbers_[i][rankingNum]->Draw();
		}

		for (int i = 0; i < 2; i++) {
		    numbers_[i + 2][rankingNum]->SetTextureHandle(textureNums_[secondsNum_[i]]);
			numbers_[i + 2][rankingNum]->SetPosition({(i + 3) * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
		    numbers_[i + 2][rankingNum]->Draw();
		}

		for (int i = 0; i < 2; i++) {
		    numbers_[i + 4][rankingNum]->SetTextureHandle(textureNums_[centiSecondsNum_[i]]);
			numbers_[i + 4][rankingNum]->SetPosition({(i + 6) * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
		    numbers_[i + 4][rankingNum]->Draw();
		}

		spriteColon_[rankingNum]->SetPosition({2 * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
	    spriteColon_[rankingNum]->Draw();

	    spriteDot_[rankingNum]->SetPosition({5 * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
	    spriteDot_[rankingNum]->Draw();

		spriteRankingDot_[rankingNum]->SetPosition({-1 * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
		spriteRankingDot_[rankingNum]->Draw();

		spriteRankingNum_[rankingNum]->SetPosition({-2 * 24.0f + numOffset_.x, 60.0f * rankingNum + numOffset_.y});
		spriteRankingNum_[rankingNum]->Draw();


		rankingNum++;
	}

	KamataEngine::Sprite::PostDraw();
}

void SceneResult::ExitScene() {

	Audio::Manager::GetInstance()->Stop(bgmHandle_);

	delete skydome_;
	delete cloud_;
}

} // namespace Atrum