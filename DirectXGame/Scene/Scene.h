#pragma once

namespace Atrum {

/// <summary>
/// シーン類 > 基底クラス
/// </summary>
class Scene {

public:
	/// <summary>
	/// コンストラクタ 空
	/// </summary>
	Scene() = default;

	/// <summary>
	/// シーン進入関数 純粋仮想
	/// </summary>
	virtual void EnterScene() = 0;

	/// <summary>
	/// 更新関数 純粋仮想
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画関数 純粋仮想
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// シーン退出関数 純粋仮想
	/// </summary>
	virtual void ExitScene() = 0;

	/// <summary>
	/// 仮想デストラクタ 空
	/// </summary>
	virtual ~Scene() = default;

	Scene(const Scene& source) = delete;
	Scene operator=(const Scene& source) = delete;
};

} // namespace Atrum