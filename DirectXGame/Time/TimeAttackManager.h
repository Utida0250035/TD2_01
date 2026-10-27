#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace Atrum {

// 表示および解釈のモード
enum class TimeFormatMode {
	MinuteSecond,    // 〇〇分〇〇秒.〇〇 （時間を分に繰り入れて計算・保持）
	HourMinuteSecond // 〇時間〇〇分〇〇秒.〇〇
};

// 時間の分解結果を保持する構造体（メンバ変数は末尾に _ を付与）
struct SplitTime {
	int32_t hours_ = 0;
	int32_t minutes_ = 0;
	int32_t seconds_ = 0;
	int32_t centiseconds_ = 0; // 小数点以下第二位まで（0〜99）

	// 総秒数（float）からモードに応じて各要素に分解する
	static SplitTime FromSeconds(float totalSeconds, TimeFormatMode mode = TimeFormatMode::MinuteSecond) {
		SplitTime st;
		// 誤差を防ぐため四捨五入して1/100秒単位の整数に変換
		int32_t totalCentisec = static_cast<int32_t>(std::round(totalSeconds * 100.0f));

		int32_t totalSecs = totalCentisec / 100;
		st.centiseconds_ = totalCentisec % 100;

		if (mode == TimeFormatMode::HourMinuteSecond) {
			st.hours_ = totalSecs / 3600;
			st.minutes_ = (totalSecs % 3600) / 60;
			st.seconds_ = totalSecs % 60;
		} else {
			// MinuteSecond モード：時間をすべて分に繰り入れる
			st.hours_ = 0;
			st.minutes_ = totalSecs / 60;
			st.seconds_ = totalSecs % 60;
		}

		return st;
	}

	// 保持している数値をもとに文字列に変換する
	std::string Format(TimeFormatMode mode) const {
		std::ostringstream oss;
		std::string centiStr = (centiseconds_ < 10 ? "0" : "") + std::to_string(centiseconds_);

		if (mode == TimeFormatMode::HourMinuteSecond) {
			oss << hours_ << "時間" << (minutes_ < 10 ? "0" : "") << minutes_ << "分" << (seconds_ < 10 ? "0" : "") << seconds_ << "秒." << centiStr;
		} else {
			oss << minutes_ << "分" << (seconds_ < 10 ? "0" : "") << seconds_ << "秒." << centiStr;
		}

		return oss.str();
	}
};

class TimeAttackManager {
private:
	float elapsedTime_ = 0.0f;       // 経過時間
	std::vector<float> rankingList_; // ランキングの記録（タイムのリスト）
	size_t rankingMaxCount_ = 3;

public:
	// 戦闘開始時にタイマーをリセット
	void StartBattle() { elapsedTime_ = 0.0f; }

	// 戦闘中、フレームごとに呼び出して時間を加算 (deltaTimeは秒単位)
	void Update(float deltaTime) { elapsedTime_ += deltaTime; }

	// 戦闘終了時に現在のタイムをランキングに追加し、昇順（早い順）に並び替える
	void EndBattle() {
		rankingList_.push_back(elapsedTime_);
		std::sort(rankingList_.begin(), rankingList_.end());

		// 必要に応じて上位10件などに絞る場合
		if (rankingList_.size() > rankingMaxCount_) {
			rankingList_.resize(rankingMaxCount_);
		}

	}

	void ToJson(const std::string& filePath) {

	}

	void FromJson(const std::string& filePath) {

	}

	SplitTime ToSplitTime() const { return SplitTime::FromSeconds(elapsedTime_, TimeFormatMode::MinuteSecond); }

	// データの取得・設定（JSON入出力用）
	float GetElapsedTime() const { return elapsedTime_; }
	const std::vector<float>& GetRankingList() const { return rankingList_; }

	void SetRankingList(const std::vector<float>& rankingList) { rankingList_ = rankingList; }
	void SetRankingMax(const size_t max) { rankingMaxCount_ = max; }
};

} // namespace Atrum