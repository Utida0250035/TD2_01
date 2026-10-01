#pragma once

#include "./Math/AeVector3.h"
#include <KamataEngine.h>

enum class MapChipType { CHIP_VOID, CHIP_BLOCK };

struct MapChipData {

	std::vector<std::vector<MapChipType>> data;
};

struct MapIndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {

public:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	// ブロックの個数
	static inline const uint32_t kBlockNumVertical = 20;
	static inline const uint32_t kBlockNumHorizontal = 100;

private:
	// マップチップ情報
	MapChipData mapChipData_;

public:
	/// <summary>
	/// マップチップ情報のリセット
	/// </summary>
	void ResetMapChipData();

	/// <summary>
	/// CSV読み込み
	/// </summary>
	/// <param name="filePath"> ファイルパス </param>
	void LoadCsv(const std::string& filePath);

	MapIndexSet GetMapCellIndex(const Atrum::Math::Vector3& position);

	/// <summary>
	/// マップチップの種類取得
	/// </summary>
	/// <param name="col"> 列番号 </param>
	/// <param name="row"> 行番号 </param>
	/// <returns> 指定セルの種類 </returns>
	MapChipType GetMapCellType(const uint32_t& col, const uint32_t row);

	/// <summary>
	/// マップチップのワールド座標取得
	/// </summary>
	/// <param name="col"> 列番号 </param>
	/// <param name="row"> 行番号 </param>
	/// <returns> 指定セルのワールド座標 </returns>
	Atrum::Math::Vector3 GetMapCellPosition(const uint32_t& col, const uint32_t row);

	Rect GetRectByIndex(const uint32_t& xIndex, const uint32_t& yIndex);
};