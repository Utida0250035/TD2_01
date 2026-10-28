#pragma once

#include <KamataEngine.h>

#include "../Audio/Audio.h"
#include "../Boss.h"
#include "../Camera/CameraController.h"
#include "../Cloud.h"
#include "../Floor.h"
#include "../Particle/CommandParticle.h"
#include "../Player.h"
#include "../Skydome.h"
#include "../Time/TimeAttackManager.h"
#include "ChangeSceneFade.h"
#include "Scene.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {

private:
	KamataEngine::Model* modelBlock_ = nullptr;
	KamataEngine::ObjectColor* blockColor_ = nullptr;

	// タイムアタックマネージャ
	std::unique_ptr<TimeAttackManager> timeAttack_ = nullptr;

	// rta記録ファイルパス
	static inline const char* kRtaRecordFilePath = "./Resources/rtaRecord.json";

	// プレイヤー
	Player* player_ = nullptr;

	// ボス
	Boss* boss_ = nullptr;

	// 天球
	Skydome* skyDome_ = nullptr;

	// 雲
	Cloud* cloud_ = nullptr;

	// ブロック
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_{};

	Floor* floor_ = nullptr;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

	// パーティクルマネージャ
	CommandParticle* commandParticle_ = nullptr;

	Audio::AudioHandle bgmHandle_{};

	std::unique_ptr<KamataEngine::Sprite> spriteSkipButton_ = nullptr;

	uint32_t textureKeyboard_ = 0;
	uint32_t textureController_ = 0;

	bool isBoss2nd_ = false;
	float scaffoldCrushTimer_ = 0.0f;
	float scaffoldCrushTime_ = 0.5f;

public:

	/// <summary>
	/// 足場崩れる演出
	/// </summary>
	/// <returns> 演出中フラグ </returns>
	bool ScaffoldCrushEffect();

	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneGame* GetInstance() {

		static SceneGame instance;

		return &instance;
	}

	// フィールド生成をまとめた関数
	void CreateField(const std::string& filePath);

	// フィールドオブジェクトの生成
	void GenerateFieldObjects();

	// フィールドオブジェクトの削除
	void RemoveFieldObjects();

	Player* GetPlayer() { return player_; }
	TimeAttackManager* GetRtaManager() { return timeAttack_.get(); }

};

} // namespace Atrum