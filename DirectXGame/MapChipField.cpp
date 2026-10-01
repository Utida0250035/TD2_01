#include "MapChipField.h"
#include "./Math/AeVector3.h"
#include <cmath>
#include <fstream>
#include <map>
#include <sstream>

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::CHIP_VOID },
    {"1", MapChipType::CHIP_BLOCK}
};

namespace M = Atrum::Math;

};

void MapChipField::ResetMapChipData() {

	mapChipData_.data.clear();
	mapChipData_.data.resize(kBlockNumVertical);

	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {

		mapChipDataLine.resize(kBlockNumHorizontal);
	}
}

void MapChipField::LoadCsv(const std::string& filePath) {

	// マップチップ情報をリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream inputFile(filePath);

#ifdef _DEBUG

	assert(inputFile);

#endif

	// CSV
	std::stringstream csv;

	// ファイルの内容を文字列ストリームにコピー
	csv << inputFile.rdbuf();

	// ファイルを閉じる
	inputFile.close();

	// csvからマップチップ情報を読み込む
	for (size_t i = 0; i < kBlockNumVertical; ++i) {

		std::string line;
		getline(csv, line);

		// 1行分の文字列をストリームに変換
		std::istringstream lineStream(line);

		for (size_t j = 0; j < kBlockNumHorizontal; ++j) {

			std::string cell;
			std::getline(lineStream, cell, ',');

			if (mapChipTable.contains(cell)) {

				mapChipData_.data[i][j] = mapChipTable[cell];
			}
		}
	}
}

MapIndexSet MapChipField::GetMapCellIndex(const M::Vector3& position) {

	M::Vector3 leftTopPos = position + M::Vector3{kBlockWidth / 2.0f, kBlockHeight / 2.0f, 0.0f};

	MapIndexSet indexSet{};
	indexSet.xIndex = static_cast<uint32_t>(leftTopPos.x / kBlockWidth);
	indexSet.yIndex = static_cast<uint32_t>(leftTopPos.y / kBlockWidth);

	indexSet.yIndex = kBlockNumVertical - 1 - indexSet.yIndex;

	return indexSet;
}

MapChipType MapChipField::GetMapCellType(const uint32_t& col, const uint32_t row) {

	if (col < 0 || col >= kBlockNumHorizontal || row < 0 || row >= kBlockNumVertical) {

		return MapChipType::CHIP_VOID;
	}

	return mapChipData_.data[row][col];
}

M::Vector3 MapChipField::GetMapCellPosition(const uint32_t& col, const uint32_t row) { return M::Vector3(kBlockWidth * col, kBlockHeight * (kBlockNumVertical - 1 - row), 0); }

Rect MapChipField::GetRectByIndex(const uint32_t& xIndex, const uint32_t& yIndex) {

	M::Vector3 center = GetMapCellPosition(xIndex, yIndex);

	Rect rect{};
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}