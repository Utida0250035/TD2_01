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

	boss_ = new Boss;
	boss_->Initialize();

	cameraController_ = new CameraController;
	cameraController_->Initialize();

	commandParticle_ = CommandParticle::GetInstance();

}

void SceneGame::Update() {

	player_->Update();

	boss_->Update();

	cameraController_->Update();

	commandParticle_->Update();

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

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_J)) {
		cameraController_->Shake(2.0f, 3.0f);
	}

	// ボス出現演出デモ
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_G)) {
		cameraController_->StartBeginBoss();
	}

#endif // _DEBUG
}

void SceneGame::Draw() { 
	player_->Draw();

	boss_->Draw();
}

void SceneGame::ExitScene() {
	delete cameraController_;
	cameraController_ = nullptr;

	delete boss_;
	boss_ = nullptr;

	delete player_;
	player_ = nullptr;

	commandParticle_ = nullptr;
}

} // namespace Atrum