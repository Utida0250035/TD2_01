#include "../Scene/SceneGame.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneResult.h"
#include <KamataEngine.h>
#include <iostream>

namespace Atrum {

void SceneGame::EnterScene() {

	std::cout << "SceneGame\n" << std::endl;

	std::cout << "key C to go Result\n" << std::endl;

	player_ = new Player;
	player_->Initialize();

	cameraController_ = new CameraController;
	cameraController_->Initialize();
}

void SceneGame::Update() {

	player_->Update();

	cameraController_->Update();

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_C)) {
		CommandChangeScene::GetInstance()->Set(SceneResult::GetInstance());
	}

#ifdef _DEBUG

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_F)) {
		if (cameraController_->GetIsFollow()) {
			cameraController_->SetIsFollow(false);
		} else {
			cameraController_->SetIsFollow(true);
			cameraController_->SetTarget({0.0f, 0.0f, 0.0f});
		}
	}

#endif // _DEBUG
}

void SceneGame::Draw() { player_->Draw(); }

void SceneGame::ExitScene() {

	delete player_;
	player_ = nullptr;
}

} // namespace Atrum