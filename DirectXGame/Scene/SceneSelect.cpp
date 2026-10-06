#include "../Scene/SceneSelect.h"
#include "../Scene/SceneGame.h"
#include "../Scene/CommandChangeScene.h"
#include <iostream>
#include <KamataEngine.h>

namespace Atrum {

void SceneSelect::EnterScene() {

	std::cout << "SceneSelect\n" << std::endl;
	std::cout << "key Space to go Game\n" << std::endl;

}

void SceneSelect::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		CommandChangeScene::GetInstance()->Set(SceneGame::GetInstance());
	}

}

void SceneSelect::Draw() {}

void SceneSelect::ExitScene() {

}

} // namespace Atrum