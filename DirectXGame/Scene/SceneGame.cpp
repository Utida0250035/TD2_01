#include "../Scene/SceneGame.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneResult.h"
#include <KamataEngine.h>
#include <iostream>

namespace Atrum {

void SceneGame::EnterScene() {

	std::cout << "SceneGame\n" << std::endl;

	std::cout << "key N to go Result\n" << std::endl;

}

void SceneGame::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_C)) {
		CommandChangeScene::GetInstance()->Set(SceneResult::GetInstance());
	}
}

void SceneGame::Draw() {

	KamataEngine::DebugText::GetInstance()->Print("SceneGame", 32, 32);

	KamataEngine::DebugText::GetInstance()->Print("key C: Go to Result(ForDebug)", 32, 64);

}

void SceneGame::ExitScene() {}

} // namespace Atrum