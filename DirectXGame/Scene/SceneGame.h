#pragma once

#include "Scene.h"
#include "../Player.h"
#include "../Boss.h"
#include "../Camera/CameraController.h"
#include "../Particle/CommandParticle.h"
#include "../Skydome.h"
#include "../Cloud.h"
#include "../Floor.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {

	private:

		KamataEngine::Model* modelBlock_ = nullptr;

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

public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneGame* GetInstance() {

		static SceneGame instance;

		return &instance;
	}

	// フィールドオブジェクトの生成
	void GenerateFieldObjects();

	Player* GetPlayer() { return player_; }

};

} // namespace Atrum