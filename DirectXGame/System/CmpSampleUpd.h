#pragma once

#include "UpdComponent.h"

namespace Atrum {

class CmpSampleUpd : UpdComponent {

public:
	/// <summary>
	/// エンティティ内でのグループ分け 大まかな実行順決定用
	/// </summary>
	/// <returns> 実行グループ(enum) </returns>
	constexpr UpdCmpGroup UpdGroup() { return UpdCmpGroup::NONE_GROUPING; }

	/// <summary>
	/// 依存解決用 オーナーエンティティからのコンポーネントのポインタ取得やシングルトンクラスのポインタ取得など
	/// </summary>
	void ResolveDependence() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

private:
	// メンバ変数

public:
	// ゲッターやセッター


};

} // namespace Atrum