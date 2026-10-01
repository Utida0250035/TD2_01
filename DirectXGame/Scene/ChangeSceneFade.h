#pragma once
#include "ChangeScene.h"
#include <KamataEngine.h>

namespace Atrum {

class ChangeSceneFade : public ChangeScene {

private:
	float timeRatio_;
	float waitCount_;
	bool isSeOpenTrigger_;
	KamataEngine::Sprite* sprite_ = nullptr;

public:
	ChangeSceneFade();

	~ChangeSceneFade() {

		delete sprite_;
		sprite_ = nullptr;

	}

	void Update() override;
	void Draw() override;
};

} // namespace Atrum