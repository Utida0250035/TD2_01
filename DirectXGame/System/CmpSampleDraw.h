#pragma once

#include "DrawComponent.h"

namespace Atrum {

class CmpSampleDraw : public DrawComponent {

public:
	// エンティティ内での実行順決定用 現在は使用しない(1グループしか無いため)
	constexpr DrawCmpGroup DrawGroup() const override { return DrawCmpGroup::NONE; }

	/// <summary>
	/// 依存解決用 エンティティからのコンポーネントのポインタ取得やシングルトンクラスのポインタ取得など
	/// </summary>
	void ResolveDependence() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

private:
	// メンバ変数など

public:
	// ゲッターやセッターなど

};

} // namespace Atrum