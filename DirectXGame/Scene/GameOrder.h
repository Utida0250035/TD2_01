#pragma once
#include "../System/WindowSize.h"
#include "../Time/DeltaTime.h"
#include "CommandChangeScene.h"
#include "CommandScene.h"
#include <KamataEngine.h>
#include <chrono>
#include <memory>

namespace Atrum {

/// <summary>
/// ゲーム統制
/// </summary>
class GameOrder final {

private:
	/// <summary>
	/// シーン指令へのアクセス
	/// </summary>
	CommandScene* commandScene_ = nullptr;

	/// <summary>
	/// シーン遷移指令へのアクセス
	/// </summary>
	CommandChangeScene* commandChangeScene_ = nullptr;

	FrameDeltaTime* frameDeltaTime_ = nullptr;

	KamataEngine::ImGuiManager* imguiManager_ = nullptr;

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameOrder();

public:
	/// <summary>
	/// 実行部分
	/// </summary>
	void Run() {

		frameDeltaTime_->CalcDeltaTime();

		if (!commandChangeScene_->GetIsExecute()) {
			// シーン遷移の処理中でなければ

			if (commandScene_->GetSceneNow()) {

				// 現在シーンの更新処理
				commandScene_->GetSceneNow()->Update();
			}

		}

		imguiManager_->Begin();

		// シーン遷移の更新処理
		commandChangeScene_->Update();

#ifdef USE_IMGUI

		imguiManager_->End();

#endif

		dxCommon_->PreDraw();

#ifdef USE_IMGUI

		imguiManager_->Draw();

#endif

		// 現在シーンの描画処理
		if (commandScene_->GetSceneNow()) {
			commandScene_->GetSceneNow()->Draw();
		}

		// シーン遷移の描画処理
		commandChangeScene_->Draw();

		dxCommon_->PostDraw();
	}

	/// <summary>
	/// デストラクタ 空
	/// </summary>
	~GameOrder() = default;

	/// <summary>
	/// コピーコンストラクタの削除
	/// </summary>
	/// <param name="source"></param>
	GameOrder(const GameOrder& source) = delete;

	/// <summary>
	/// 代入演算子のオーバーロードの削除
	/// </summary>
	/// <param name="source"></param>
	/// <returns></returns>
	GameOrder& operator=(const GameOrder& source) = delete;

	/// <summary>
	///
	/// </summary>
	/// <returns> ゲーム統制の静的インスタンス </returns>
	static GameOrder* GetInstance() {

		static GameOrder instance;

		return &instance;
	}
};

} // namespace Atrum