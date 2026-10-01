#include "GameOrder.h"
#include "CommandChangeScene.h"
#include "CommandScene.h"
#include <chrono>

namespace Atrum {
/// <summary>
/// ゲーム統制 > コンストラクタ
/// </summary>
GameOrder::GameOrder() {

	// シーン指令のインスタンス取得
	commandScene_ = CommandScene::GetInstance();

	commandChangeScene_ = CommandChangeScene::GetInstance();

	frameDeltaTime_ = FrameDeltaTime::GetInstance();

	imguiManager_ = KamataEngine::ImGuiManager::GetInstance();

	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();

}

} // namespace Atrum