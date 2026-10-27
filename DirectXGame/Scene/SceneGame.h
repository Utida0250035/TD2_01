#pragma once

#include "Scene.h"
#include "../Player.h"
#include "../Boss.h"
#include "../Camera/CameraController.h"
#include "../Particle/CommandParticle.h"
#include "../Skydome.h"
#include "../Cloud.h"
#include "../Floor.h"
#include "../Audio/Audio.h"

#include "ChangeSceneFade.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {

	private:

		KamataEngine::Model* modelBlock_ = nullptr;
	    KamataEngine::ObjectColor* blockColor_ = nullptr;

		// プレイヤー
	    Player* player_ = nullptr;

		// ボス
	    Boss* boss_ = nullptr;

		// 天球
	    Skydome* skyDome_ = nullptr;

		// 雲
	    Cloud* cloud_ = nullptr;

		// ブロック
	    std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

		Floor* floor_ = nullptr;

		// カメラコントローラ
	    CameraController* cameraController_ = nullptr;

		// パーティクルマネージャ
		CommandParticle* commandParticle_ = nullptr;

		Audio::AudioHandle bgmHandle_;

		bool isBoss2nd_ = false;

public:
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

};

} // namespace Atrum