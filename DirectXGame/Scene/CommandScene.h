#pragma once

#include "Scene.h"
#include <cassert>

namespace Atrum {

/// <summary>
/// 指令類 > シーン指令
///
/// シーンごとの処理の指令クラス
/// </summary>
class CommandScene final {

private:
	/// <summary>
	/// 現在シーン
	/// </summary>
	Scene* sceneNow_ = nullptr;

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandScene() = default; // 空

public:
	/// <summary>
	/// コピーコンストラクタの削除
	/// </summary>
	/// <param name="source"></param>
	CommandScene(const CommandScene& source) = delete;

	/// <summary>
	/// 代入演算子のオーバーロードの削除
	/// </summary>
	/// <param name="source"></param>
	/// <returns></returns>
	CommandScene& operator=(const CommandScene& source) = delete;

	/// <summary>
	/// (注; ゲーム指令とシーン遷移指令でのみ参照)
	/// </summary>
	/// <returns> シーンマネージャーの静的インスタンス </returns>
	static CommandScene* GetInstance() {

		static CommandScene instance;

		return &instance;
	}

	/// <summary>
	/// シーン遷移 (注: 直接の実行禁止 CommandChangeSceneクラスのUpdate関数でのみ参照)
	/// </summary>
	/// <param name="sceneNext"> 次のシーン </param>
	void Transition(Scene* sceneNext) {

		assert(sceneNext);

		if (sceneNext) {

			if (sceneNow_) {
				// 現在シーンの退出処理
				sceneNow_->ExitScene();
			}

			// 現在シーンの変更
			sceneNow_ = sceneNext;

			// 現在シーン(変更済)の進入処理
			sceneNow_->EnterScene();
		}
	}

	/// <summary>
	/// (注: ゲームマネージャーでの処理にのみ使用)
	/// </summary>
	/// <returns> 現在シーンのインスタンス </returns>
	Scene* GetSceneNow() const { return sceneNow_; }
};

} // namespace Atrum