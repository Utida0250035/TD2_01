#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Math/Vector3.h"

/// <summary>
/// 床
/// </summary>
class Floor {
public:
	enum class MapChipType {
		kBlank,  // 空白
		kBlock,  // ブロック
	};

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 範囲矩形
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 25;
	static inline const uint32_t kNumBlockHorizontal = 50;

	// マップの開始基準座標（左下基準）
	static inline const float kOriginX = -50.0f;
	static inline const float kOriginY = -20.0f;

	// マップチップCSVの文字番号
	enum MapChipCharIndex {
		kChipType = 0,  // マップチップタイプ
		kChipSubID = 1, // タイプごとのサブID
	};

	// 1マス分のデータ
	struct MapChipDataUnit {
		MapChipType type; // マップチップの種別
		uint8_t subID;    // 種類ごとのサブID
	};

	// ステージ全体のマップチップデータ
	struct MapChipData {
		std::vector<std::vector<MapChipDataUnit>> data;
	};

	MapChipData mapChipData_;

public:
	// リセット
	void ResetMapChipData();

	// 読み込み
	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	uint8_t GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex);
	Atrum::Math::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	IndexSet GetMapChipIndexSetByPosition(const Atrum::Math::Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();
};
