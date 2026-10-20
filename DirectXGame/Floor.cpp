#include "Floor.h"
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>

namespace {

std::map<char, Floor::MapChipType> mapChipTypeTable = {
    {'B', Floor::MapChipType::kBlock},
};

}

void Floor::ResetMapChipData() {
	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipDataUnit>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void Floor::LoadMapChipCsv(const std::string& filePath) {
	// リセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		std::string line;
		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換
		std::istringstream lineStream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			std::string word;
			std::getline(lineStream, word, ',');

			// 空白はスキップ
			if (word.empty()) {
				continue;
			}

			// 先頭文字がいずれかのマップチップ種別に該当しないならスキップ
			if (!mapChipTypeTable.contains(word[kChipType])) {
				continue;
			}

			// 先頭文字でマップチップのタイプを判別
			mapChipData_.data[i][j].type = mapChipTypeTable[word[kChipType]];

			// サブIDを含まない場合はスキップ
			if (word.size() <= kChipSubID) {
				continue;
			}

			// マップチップのサブIDを設定
			mapChipData_.data[i][j].subID = static_cast<uint8_t>(word[kChipSubID] - '0');
		}
	}
}

Floor::MapChipType Floor::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex].type;
}

uint8_t Floor::GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex) { return mapChipData_.data[yIndex][xIndex].subID; }

Atrum::Math::Vector3 Floor::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return Atrum::Math::Vector3(kOriginX + kBlockWidth * xIndex, kOriginY + kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0.0f);
}

Floor::IndexSet Floor::GetMapChipIndexSetByPosition(const Atrum::Math::Vector3& position) {
	IndexSet result = {};

	result.xIndex = static_cast<uint32_t>(std::floor((position.x - kOriginX + kBlockWidth / 2.0f) / kBlockWidth));
	int yFromBottom = static_cast<int>(std::floor((position.y - kOriginY + kBlockHeight / 2.0f) / kBlockHeight));
	result.yIndex = static_cast<uint32_t>(static_cast<int>(kNumBlockVirtical) - 1 - yFromBottom);

	return result;
}

Floor::Rect Floor::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) { // 指定ブロックの中心座標を取得する
	Atrum::Math::Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}

uint32_t Floor::GetNumBlockVirtical() { return kNumBlockVirtical; }

uint32_t Floor::GetNumBlockHorizontal() { return kNumBlockHorizontal; }
