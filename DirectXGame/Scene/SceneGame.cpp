#include "../Scene/SceneGame.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneResult.h"
#include "../SEPlayer.h"
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
	cameraController_->StartBeginBoss();

	commandParticle_ = CommandParticle::GetInstance();

}

void SceneGame::Update() {

	player_->Update();

	boss_->Update();

	cameraController_->Update();

	commandParticle_->Update();

	SEPlayer::GetInstance()->Update();

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_C)) {
		CommandChangeScene::GetInstance()->Set(SceneResult::GetInstance());
	}

#ifdef _DEBUG

	

#endif // _DEBUG
}

void SceneGame::Draw() { 
	player_->Draw();

	boss_->Draw();

	player_->DrawUI();
	boss_->DrawUI();

}

void SceneGame::ExitScene() {
	delete cameraController_;
	cameraController_ = nullptr;

	boss_->GetEntity()->Finalize();
	delete boss_;
	boss_ = nullptr;

	delete player_;
	player_ = nullptr;

	commandParticle_ = nullptr;
}

} // namespace Atrum