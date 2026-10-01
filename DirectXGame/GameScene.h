#pragma once
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "HitEffect.h"
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include "./Audio/AudioHandle.h"
#include <memory>
#include <vector>

namespace Atrum {

	class FrameDeltaTime;

}

class CameraController;
class MapChipField;
class Player;

class GameScene {
public:
	enum class Phase { 
		kFadeIn,
		kPlay,
		kDeath,
		kFadeOut
	};

private:

	Phase phase_ = Phase::kFadeIn;

	std::unique_ptr<Fade> fade_ = nullptr;

	inline static constexpr float kFadeDuration = 1.0f;

	/// <summary>
	/// 自機
	/// </summary>
	Player* player_ = nullptr;

	/// <summary>
	/// ブロックの3Dモデル
	/// </summary>
	KamataEngine::Model* blockModel_ = nullptr;

	/// <summary>
	/// ブロック群のワールド座標変換
	/// </summary>
	std::vector<std::vector<KamataEngine::WorldTransform*>> blocksWorldTransform_;

	/// <summary>
	/// カメラ
	/// </summary>
	KamataEngine::Camera* camera_ = nullptr;

	/// <summary>
	/// デバッグカメラの有効フラグ
	/// </summary>
	bool isDebugCameraActive_ = false;

	/// <summary>
	/// デバッグカメラ
	/// </summary>
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// 天球
	/// </summary>
	std::unique_ptr<Skydome> skydome_ = nullptr;

	/// <summary>
	/// 天球の3Dモデル
	/// </summary>
	KamataEngine::Model* skydomeModel_ = nullptr;

	/// <summary>
	/// マップチップフィールド
	/// </summary>
	MapChipField* mapChipField_ = nullptr;

	/// <summary>
	/// 敵
	/// </summary>
	std::list<std::unique_ptr<Enemy>> enemies_{};

	/// <summary>
	/// ヒットエフェクト
	/// </summary>
	std::list<std::unique_ptr<HitEffect>> hitEffects_{};

	/// <summary>
	/// ブロックの生成
	/// </summary>
	void GenerateBlocks();

	/// <summary>
	/// 時間インスタンス
	/// </summary>
	Atrum::FrameDeltaTime* time_ = nullptr;

	/// <summary>
	/// 実経過時間
	/// </summary>
	float deltaTime_ = 0.0f;

	/// <summary>
	/// カメラコントローラー
	/// </summary>
	CameraController* cameraController_ = nullptr;

	std::unique_ptr<DeathParticles> playerDeathParticles_ = nullptr;

	Atrum::Audio::AudioHandle bgmPlayHandle_{};

	void CheckAllCollisions();

	void UpdateHitEffects();
	void UpdateEnemies();
	void UpdateBlocks();
	void UpdatePlayer();
	void UpdatePlayerDeathParticles();
	void UpdateCamera();
	void UpdateCameraControler();
	void ChangePhase();

	bool isFinished_ = false;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();


	/* ゲッター */

	bool GetIsFinished() const { return isFinished_; }

};