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

	// ボス演出カメラ
	if (boss_->GetIsDeath()) {
		cameraController_->StartEndBoss(boss_->GetEntity()->GetWorldPosition());
	}

	if (boss_->IsDeathFinished()) {
		CommandChangeScene::GetInstance()->Set(SceneResult::GetInstance());
		commandParticle_->DestroyParticleAll();
	}

	if (player_->IsDeathFinished()) {
		CommandChangeScene::GetInstance()->Set(SceneGame::GetInstance(), new ChangeSceneFade);
		commandParticle_->DestroyParticleAll();
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

			modelBlock_->Draw(*worldTransformBlock, *KeMainCamera::GetInstance()->camera_);
		}
	}

	KamataEngine::Model::PostDraw();

	player_->Draw();

	boss_->Draw();


	player_->DrawUI();
	boss_->DrawUI();

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

	delete cloud_;

	commandParticle_ = nullptr;

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