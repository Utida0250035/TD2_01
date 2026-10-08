#pragma once

#include "Scene.h"
#include "../Player.h"
#include "../Camera/CameraController.h"
#include "../Particle/CommandParticle.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {

	private:

		// プレイヤー
	    Player* player_ = nullptr;

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
};

} // namespace Atrum