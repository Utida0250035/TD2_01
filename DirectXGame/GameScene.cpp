#include "GameScene.h"
#include "./Audio/Audio.h"
#include "AABB.h"
#include "CameraController.h"
#include "./Time/DeltaTime.h"
#include "Enemy.h"
#include "KeMatrix3D.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "WindowSize.h"

namespace {

	namespace M = Atrum::Math;

}

void GameScene::GenerateBlocks() {

	// ブロックの要素数を反映
	blocksWorldTransform_.resize(mapChipField_->kBlockNumVertical);

	for (uint32_t i = 0; i < mapChipField_->kBlockNumVertical; ++i) {

		blocksWorldTransform_[i].resize(mapChipField_->kBlockNumHorizontal);

		for (uint32_t j = 0; j < mapChipField_->kBlockNumHorizontal; ++j) {

			if (mapChipField_->GetMapCellType(j, i) == MapChipType::CHIP_VOID) {
				// セル種類が空なら生成しない
				continue;
			}

			// ブロックを生成
			blocksWorldTransform_[i][j] = new KamataEngine::WorldTransform();
			blocksWorldTransform_[i][j]->Initialize();
			blocksWorldTransform_[i][j]->translation_.x = mapChipField_->kBlockWidth * j;
			blocksWorldTransform_[i][j]->translation_.y = mapChipField_->kBlockHeight * (mapChipField_->kBlockNumVertical - 1 - i);
		}
	}
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵キャラの当たり判定

	//===================================
	// 自キャラと敵キャラの当たり判定
	//===================================

	AABB playerAABB = player_->GetAABB();

	AABB enemyAABB{};

	for (auto& enemy : enemies_) {

		enemyAABB = enemy->GetAABB();

		if (IsCollisionAABB(playerAABB, enemyAABB)) {

			if (!enemy->GetIscollisionAble()) {
				continue;
			}

			if (player_->isAttack()) {

				enemy->SetIsCollisionAble(false);
				hitEffects_.emplace_back(HitEffect::Create(enemy->GetPosWorld(), 0.25f));

			} else {

				player_->SetIsAlive(false);
			}

			break;
		}
	}

#pragma endregion
}

void GameScene::Initialize() {

	phase_ = Phase::kFadeIn;

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeDuration);

	camera_ = new KamataEngine::Camera();
	camera_->Initialize();

	// 自機を生成
	player_ = new Player();

	M::Vector3 playerPosition = mapChipField_->GetMapCellPosition(0, 18);

	// 自機を初期化
	player_->Initialize(
	    KamataEngine::Model::CreateFromOBJ("playerHako", true), KamataEngine::TextureManager::GetInstance()->Load("./Resources/green4x4.png"), KamataEngine::Model::CreateFromOBJ("katana"), camera_,
	    playerPosition);

	// 自機の死亡パーティクルを生成
	playerDeathParticles_.reset(new DeathParticles);

	// 自機の死亡パーティク<ルを初期化
	playerDeathParticles_->Initialize(
	    KamataEngine::Model::CreateFromOBJ("deathParticle", true), KamataEngine::TextureManager::GetInstance()->Load("./Resources/deathParticle/white1x1.png"), camera_, playerPosition);

	// ブロックの3Dモデルを生成
	blockModel_ = KamataEngine::Model::CreateFromOBJ("blockRounded");

	// デバッグカメラを生成
	debugCamera_ = new KamataEngine::DebugCamera(static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// 天球3Dモデルを生成
	skydomeModel_ = KamataEngine::Model::CreateFromOBJ("SkyDome", true);

	// 天球を生成
	skydome_.reset(new Skydome());
	skydome_->Initialize(skydomeModel_, camera_);

	// マップチップフィールドを読み込み
	mapChipField_ = new MapChipField;
	mapChipField_->LoadCsv("./Resources/tileMap.csv");
	player_->SetMapChipField(mapChipField_);

	// ブロックを生成
	GenerateBlocks();

	// 時間インスタンスを取得
	time_ = Atrum::FrameDeltaTime::GetInstance();

	// カメラコントローラーを生成
	cameraController_ = new CameraController();

	cameraController_->SetTargetOffset(M::Vector3{0.0f, 0.0f, -10.0f});
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	cameraController_->CameraReset();
	cameraController_->SetMovableArea({0.0f, 64.0f, 0.0f, 64.0f});

	std::vector<M::Vector3> enemyPosition = {mapChipField_->GetMapCellPosition(20, 10), mapChipField_->GetMapCellPosition(17, 10), mapChipField_->GetMapCellPosition(15, 10)};

	for (size_t i = 0; i < enemyPosition.size(); i++) {

		std::unique_ptr<Enemy> newEnemy = std::unique_ptr<Enemy>(new Enemy());

		newEnemy->Initialize(
		    KamataEngine::Model::CreateFromOBJ("enemyFoxMask", true), KamataEngine::TextureManager::GetInstance()->Load("./Resources/enemyFoxMask/enemy.png"), camera_, enemyPosition[i]);
		newEnemy->SetMapChipField(mapChipField_);

		enemies_.push_back(std::move(newEnemy));
	}

	HitEffect::SetCamera(camera_);

	HitEffect::SetModel(KamataEngine::Model::CreateFromOBJ("particle", true));

	time_->Initialize();

	bgmPlayHandle_ = Atrum::Audio::Manager::GetInstance()->Play(Atrum::Audio::Manager::GetInstance()->Get("./Resources/bgmYasha.mp3"), true);
}

void GameScene::UpdateHitEffects() {

	hitEffects_.remove_if([](std::unique_ptr<HitEffect>& effect) {
		if (effect->GetIsFinish()) {
			effect.reset();
			return true;
		}

		return false;
	});

	for (auto& effect : hitEffects_) {

		if (!effect) {
			continue;
		}

		effect->Update(deltaTime_);
	}
}

void GameScene::UpdateEnemies() {

	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (!enemy->GetIsAlive()) {
			enemy.reset();
			return true;
		}

		return false;
	});

	for (auto& enemy : enemies_) {

		if (enemy) {
			// 敵を更新
			enemy->Update(deltaTime_);
		}
	}
}

void GameScene::UpdateBlocks() {

	// ブロックを更新
	for (std::vector<KamataEngine::WorldTransform*>& blockWorldTransformLine : blocksWorldTransform_) {

		for (KamataEngine::WorldTransform* blockWorldTransform : blockWorldTransformLine) {

			if (!blockWorldTransform) {
				continue;
			}

			// ワールド変換行列を作成
			blockWorldTransform->matWorld_ = MakeWorldMatrix(blockWorldTransform->translation_, blockWorldTransform->scale_, blockWorldTransform->rotation_);

			// 定数バッファに転送する
			blockWorldTransform->TransferMatrix();
		}
	}
}

void GameScene::UpdatePlayer() {

	// 自機を更新
	player_->Update(deltaTime_);
}

void GameScene::UpdatePlayerDeathParticles() {

	if (playerDeathParticles_) {

		playerDeathParticles_->Update(deltaTime_);
	}
}

void GameScene::UpdateCamera() {

	// デバッグカメラを更新
	debugCamera_->Update();

#ifdef _DEBUG

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_C)) {

		if (isDebugCameraActive_) {

			isDebugCameraActive_ = false;

		} else {

			isDebugCameraActive_ = true;
		}
	}

#endif

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;

		// ビュープロジェクション行列の転送
		camera_->TransferMatrix();
	}
}

void GameScene::UpdateCameraControler() {

	if (!isDebugCameraActive_) {

		cameraController_->Update();
	}
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:

		if (!player_->GetIsAlive()) {

			phase_ = Phase::kDeath;

			M::Vector3 deathParticlePosition = player_->GetWorldPosition();

			playerDeathParticles_.reset(new DeathParticles);

			playerDeathParticles_->Initialize(
			    KamataEngine::Model::CreateFromOBJ("deathParticle", true), KamataEngine::TextureManager::GetInstance()->Load("./Resources/deathParticle/white1x1.png"), camera_, deathParticlePosition);
		}

		break;

	case Phase::kDeath:

		break;
	}
}

void GameScene::Update() {

	// 実経過時間を取得
	deltaTime_ = time_->GetDeltaTime();

	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:

		UpdateBlocks();

		UpdatePlayer();

		UpdateEnemies();

		UpdateHitEffects();

		UpdateCameraControler();

		UpdateCamera();

		if (fade_->IsFinished()) {

			phase_ = Phase::kPlay;
		}

		break;

	case Phase::kPlay:

		UpdateBlocks();

		UpdatePlayer();

		UpdateEnemies();

		UpdateHitEffects();

		CheckAllCollisions();

		UpdateCameraControler();

		UpdateCamera();

		break;

	case Phase::kDeath:

		UpdateBlocks();

		UpdatePlayerDeathParticles();

		UpdateEnemies();

		CheckAllCollisions();

		UpdateCamera();

		break;

	case Phase::kFadeOut:

		if (fade_->IsFinished()) {

			isFinished_ = true;
		}

		break;
	}

	if (playerDeathParticles_ && playerDeathParticles_->GetIsFinished()) {

		playerDeathParticles_.reset();

		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, kFadeDuration);
	}

	fade_->Update();
}

void GameScene::Draw() {

	KamataEngine::Model::PreDraw();

	skydome_->Draw();

	// ブロックを描画
	for (std::vector<KamataEngine::WorldTransform*>& blockWorldTransformLine : blocksWorldTransform_) {

		for (KamataEngine::WorldTransform* blockWorldTransform : blockWorldTransformLine) {

			if (!blockWorldTransform) {
				continue;
			}

			blockModel_->Draw(*blockWorldTransform, *camera_);
		}
	}

	if (playerDeathParticles_) {

		playerDeathParticles_->Draw();
	}

	// 自機を描画
	player_->Draw();

	for (const auto& enemy : enemies_) {
		if (enemy) {

			// 敵を描画
			enemy->Draw();
		}
	}

	for (const auto& effect : hitEffects_) {

		if (!effect) {
			continue;
		}

		effect->Draw();
	}

	KamataEngine::Model::PostDraw();

	KamataEngine::Sprite::PreDraw();

	fade_->Draw();

	KamataEngine::Sprite::PostDraw();
}

GameScene::~GameScene() {

	// 自機を削除
	delete player_;
	player_ = nullptr;

	// ブロックの3Dモデルデータを削除
	delete blockModel_;
	blockModel_ = nullptr;

	// ブロックの座標変換データを削除
	for (std::vector<KamataEngine::WorldTransform*>& blockWorldTransformLine : blocksWorldTransform_) {

		for (KamataEngine::WorldTransform* blockWorldTransform : blockWorldTransformLine) {

			delete blockWorldTransform;
		}
	}

	blocksWorldTransform_.erase(blocksWorldTransform_.begin(), blocksWorldTransform_.end());

	// デバッグカメラを削除
	delete debugCamera_;
	debugCamera_ = nullptr;

	// 天球3Dモデルを削除
	delete skydomeModel_;
	skydomeModel_ = nullptr;

	// マップチップフィールドを削除
	delete mapChipField_;
	mapChipField_ = nullptr;

	Atrum::Audio::Manager::GetInstance()->Stop(bgmPlayHandle_);

}