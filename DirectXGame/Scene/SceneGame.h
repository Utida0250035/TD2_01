#pragma once

#include "Scene.h"
#include "../Player.h"
#include "../Boss.h"
#include "../Camera/CameraController.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {

	private:

		// プレイヤー
	    Player* player_ = nullptr;

		// ボス
	    Boss* boss_ = nullptr;

		// カメラコントローラ
	    CameraController* cameraController_ = nullptr;

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