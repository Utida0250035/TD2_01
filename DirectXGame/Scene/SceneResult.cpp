#include "../Scene/SceneResult.h"
#include "../Scene/CommandChangeScene.h"
#include "../Scene/SceneGame.h"
#include "../Scene/SceneSelect.h"
#include "../Scene/SceneTitle.h"
#include <KamataEngine.h>
#include <iostream>

namespace Atrum {

void SceneResult::EnterScene() { 
	choice_ = 0;

	std::cout << "key A,D to select destination\n" << std::endl;

	std::cout << "key Space to go nextLevel\n" << std::endl;

}

void SceneResult::Update() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_A)) {
		choice_ -= 1;
	}

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_D)) {
		choice_ += 1;
	}

	if (choice_ < 0) {

		choice_ = kChoiceMax;

	} else if (choice_ > kChoiceMax) {

		choice_ = 0;
	}

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_A) || KamataEngine::Input::GetInstance()->TriggerKey(DIK_D)) {

		switch (choice_) {

		case 0:
			// 次のレベルへ

			// 遷移先テキスト SceneGame
			std::cout << "key Space to go nextLevel\n" << std::endl;

			break;

		case 1:
			// ステージセレクトへ

			// 遷移先テキスト SceneSelect
			std::cout << "key Space to go levelSelect\n" << std::endl;

			break;

		case 2:
			// タイトルへ

			// 遷移先テキスト SceneTitle
			std::cout << "key Space to go title\n" << std::endl;

			break;
		}
	}

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		switch (choice_) {

		case 0:
			// 次のレベルへ

			// レベル制御

			// SceneGameへの遷移
			CommandChangeScene::GetInstance()->Set(SceneGame::GetInstance());

			break;

		case 1:
			// ステージセレクトへ

			// SceneSelectへの遷移
			CommandChangeScene::GetInstance()->Set(SceneSelect::GetInstance());

			break;

		case 2:
			// タイトルへ

			// SceneTitleへの遷移
			CommandChangeScene::GetInstance()->Set(SceneTitle::GetInstance());

			break;
		}
	}
}

void SceneResult::Draw() {}

void SceneResult::ExitScene() {}

} // namespace Atrum