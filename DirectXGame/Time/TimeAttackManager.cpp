#include "TimeAttackManager.h"
#include "../Json/JsonHandler.h"

namespace Atrum {

	void TimeAttackManager::ToJson(const std::string& filePath) const { Json::JsonHandler::SaveToFile<std::vector<float>>(filePath, rankingList_); }

	void TimeAttackManager::FromJson(const std::string& filePath) { rankingList_ = Json::JsonHandler::LoadFromFile<std::vector<float>>(filePath); }

}