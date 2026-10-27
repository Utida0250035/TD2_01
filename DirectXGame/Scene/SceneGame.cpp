#include "../Scene/SceneGame.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneResult.h"
#include "../SEPlayer.h"
#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"
#include <KamataEngine.h>
#include <iostream>

using namespace KamataEngine;

namespace Atrum {

void SceneGame::EnterScene() {

	std::cout << "SceneGame\n" << std::endl;

	std::cout << "key C to go Result\n" << std::endl;

	modelBlock_ = Model::CreateFromOBJ("blockRounded", true);
	blockColor_ = new KamataEngine::ObjectColor();
	blockColor_->Initialize();
	blockColor_->SetColor({0.5f, 0.5f, 0.5f, 0.75f});

	player_ = new Player;
	player_->Initialize();

	boss_ = new Boss;
	boss_->Initialize();

	skyDome_ = new Skydome;
	skyDome_->Initialize(KamataEngine::Model::CreateFromOBJ("skyDome", true), KeMainCamera::GetInstance()->camera_);

	cloud_ = new Cloud;
	cloud_->Initialize(KamataEngine::Model::CreateFromOBJ("cloud"), KeMainCamera::GetInstance()->camera_);

	// 床
	floor_ = new Floor;
	floor_->LoadMapChipCsv("./Resources/field.csv");
	player_->SetFloor(floor_);

	GenerateFieldObjects();

	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->StartBeginBoss();
	boss_->SetCameraController(cameraController_);

	commandParticle_ = CommandParticle::GetInstance();

	bgmHandle_ = Audio::Manager::GetInstance()->Play("./Resources/bgmGame.mp3", true);

	boss_->SetPlayerTranslate(player_->GetTranslatePointer());

	textureKeyboard_ = KamataEngine::TextureManager::Load("skipManualK.png");
	textureController_ = KamataEngine::TextureManager::Load("skipManualC.png");
	if (KamataEngine::Input::GetInstance()->GetNumberOfJoysticks()) {
		spriteSkipButton_.reset(KamataEngine::Sprite::Create(textureController_, {1050.0f, 50.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	} else {
		spriteSkipButton_.reset(KamataEngine::Sprite::Create(textureKeyboard_, {1050.0f, 50.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	}

	Update();

}

void SceneGame::Update() {

	player_->Update();

	boss_->Update();

	skyDome_->Update();

	cloud_->Update();

	cameraController_->Update();

	commandParticle_->Update();

	SEPlayer::GetInstance()->Update();

	#ifdef _DEBUG

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_C)) {
		CommandChangeScene::GetInstance()->Set(SceneResult::GetInstance());
		commandParticle_->DestroyParticleAll();
	}

	#endif

	XINPUT_STATE state{};
	XINPUT_STATE preState{};
	if (KamataEngine::Input::GetInstance()->GetJoystickState(0, state)) {
		KamataEngine::Input::GetInstance()->GetJoystickStatePrevious(0, preState);
	}

	// 演出スキップ
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE) || (state.Gamepad.wButtons == XINPUT_GAMEPAD_A && preState.Gamepad.wButtons != XINPUT_GAMEPAD_A)) {
		boss_->SkipBegin();
	}

	// ボス演出カメラ
	if (boss_->GetIsDeath()) {
		cameraController_->StartEndBoss(boss_->GetEntity()->GetWorldPosition());
		player_->SetIsDeathBoss(true);
	}

	if (boss_->IsDeathFinished()) {
		CommandChangeScene::GetInstance()->Set(SceneResult::GetInstance(), new ChangeSceneFade());
		commandParticle_->DestroyParticleAll();
	}

	if (player_->IsDeathFinished()) {
		CommandChangeScene::GetInstance()->Set(SceneGame::GetInstance(), new ChangeSceneFade);
		commandParticle_->DestroyParticleAll();
	}

	if (KamataEngine::Input::GetInstance()->GetNumberOfJoysticks()) {
		spriteSkipButton_->SetTextureHandle(textureController_);
	} else {
		spriteSkipButton_->SetTextureHandle(textureKeyboard_);
	}

#ifdef _DEBUG

	

#endif // _DEBUG
}

void SceneGame::Draw() {
	KamataEngine::Model::PreDraw();

	skyDome_->Draw();

	cloud_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			modelBlock_->Draw(*worldTransformBlock, *KeMainCamera::GetInstance()->camera_, blockColor_);
		}
	}

	KamataEngine::Model::PostDraw();

	player_->Draw();

	boss_->Draw();

	player_->DrawUI();
	boss_->DrawUI();

	KamataEngine::Sprite::PreDraw();
	if (CommandParticle::GetInstance()->IsExist("bossAppear")) {
		spriteSkipButton_->Draw();
	}
	KamataEngine::Sprite::PostDraw();
}

void SceneGame::ExitScene() {
	delete cameraController_;
	cameraController_ = nullptr;

	boss_->GetEntity()->Finalize();
	delete boss_;
	boss_ = nullptr;

	delete player_;
	player_ = nullptr;

	delete skyDome_;
	skyDome_ = nullptr;

	delete cloud_;
	cloud_ = nullptr;

	commandParticle_ = nullptr;

	delete blockColor_;
	blockColor_ = nullptr;

	Audio::Manager::GetInstance()->Stop(bgmHandle_);
}

void SceneGame::GenerateFieldObjects() {
	// 要素数
	uint32_t kNumBlockVirtical = floor_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = floor_->GetNumBlockHorizontal();

	// 列数
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		// 1列の要素数
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// フィールドオブジェクトの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			WorldTransform* worldTransform = new WorldTransform();
			worldTransform->Initialize();

			Floor::MapChipType mapChipType = floor_->GetMapChipTypeByIndex(j, i);
			switch (mapChipType) {
			case Floor::MapChipType::kBlock:
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = ToKamataEngine(floor_->GetMapChipPositionByIndex(j, i));
				worldTransformBlocks_[i][j]->scale_ = {2.0f, 2.0f, 2.0f};
				UpdateWorldTransform(*worldTransform); 
				break;
			}
		}
	}
}

} // namespace Atrum