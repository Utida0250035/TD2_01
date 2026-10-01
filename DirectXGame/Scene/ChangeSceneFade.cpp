#include "../Scene/ChangeSceneFade.h"
#include "../Audio/Audio.h"
#include "../Easing.h"
#include "GameOrder.h"
#include "../Time/DeltaTime.h"

namespace Atrum {

ChangeSceneFade::ChangeSceneFade() {

	timeRatio_ = 0.0f;
	waitCount_ = 0.0f;
	isSeOpenTrigger_ = false;

	sprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::GetInstance()->Load("./Resources/white1x1.png"), {}, {0.0f, 0.0f, 0.0f, 1.0f});
	sprite_->SetSize({1280.0f, 720.0f});
}

void ChangeSceneFade::Update() {

	if (!isProcess_) {

		isProcess_ = true;
	}

	if (isSceneChange_) {

		waitCount_ += Atrum::FrameDeltaTime::GetInstance()->GetDeltaTime();

		if (waitCount_ >= 0.25f) {

			if (!isSeOpenTrigger_) {

				isSeOpenTrigger_ = true;
			}

			waitCount_ = 1.0f;

			timeRatio_ -= 1.0f / 0.25f * FrameDeltaTime::GetInstance()->GetDeltaTime();

			if (timeRatio_ <= 0.0f) {

				isProcess_ = false;
			}
		}

	} else {

		timeRatio_ += 1.0f / 0.5f * FrameDeltaTime::GetInstance()->GetDeltaTime();

		if (timeRatio_ >= 1.0f) {

			timeRatio_ = 1.0f;

			isSceneChange_ = true;
		}
	}
}

void ChangeSceneFade::Draw() {

	KamataEngine::Sprite::PreDraw();

	sprite_->SetColor({0.0f, 0.0f, 0.0f, Atrum::Interpolation::EaseInCirc(timeRatio_)});

	sprite_->Draw();

	KamataEngine::Sprite::PostDraw();
}

} // namespace Atrum