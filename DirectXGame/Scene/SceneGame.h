#pragma once

#include "Scene.h"
#include "../Player.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {

	private:

		// プレイヤー
	    Player* player_ = nullptr;

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