#include "../Scene/SceneTitle.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneSelect.h"
#include <KamataEngine.h>
#include <iostream>

namespace Atrum {

void SceneTitle::EnterScene() {

	std::cout << "TitleScene\n" << std::endl;
	std::cout << "key Space to go Select\n" << std::endl;

}

void SceneTitle::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		CommandChangeScene::GetInstance()->Set(SceneSelect::GetInstance());
	}
}

void SceneTitle::Draw() {

}

void SceneTitle::ExitScene() {}

} // namespace Atrum