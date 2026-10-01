#include "CommandChangeScene.h"
#include "CommandScene.h"

namespace Atrum {

/// <summary>
/// 指令類 > シーン遷移指令 > コンストラクタ
/// </summary>
CommandChangeScene::CommandChangeScene() {

	commandScene_ = CommandScene::GetInstance();
	nextScene_ = nullptr;
	changeSceneEffect_.reset();
	isExecute_ = false;
}

} // namespace Atrum