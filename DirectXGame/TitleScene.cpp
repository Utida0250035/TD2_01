#include "TitleScene.h"
#include "./Time/DeltaTime.h"
#include "Easing.h"
#include "./Math/Lerp.h"
#include "KeMatrix3D.h"
#include <algorithm>
#include <numbers>

namespace {

namespace I = Atrum::Interpolation;
namespace A = Atrum;

} // namespace

void TitleScene::Initialize() {

	camera_ = new KamataEngine::Camera();
	camera_->Initialize();
	camera_->translation_ = {0.0f, 1.0f, -30.0f};

	playerModel_ = KamataEngine::Model::CreateFromOBJ("playerHako");

	titleLogoModel_ = KamataEngine::Model::CreateFromOBJ("titleFont");

	currentT_ = 0.0f;

	titleLogoWorldTransform_.Initialize();
	titleLogoWorldTransform_.translation_ = {0.0f, 8.0f, 0.0f};

	playerWorldTransform_.Initialize();
	playerWorldTransform_.translation_ = {0.0f, 0.0f, -3.0f};
	playerWorldTransform_.rotation_ = {0.0f, std::numbers::pi_v<float>, 0.0f};

	phase_ = Phase::kFadeIn;
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeDuration);
}

void TitleScene::Update() {

	deltaTime_ = A::FrameDeltaTime::GetInstance()->GetDeltaTime();

	currentT_ += tSpeed_ * deltaTime_;

	if (currentT_ <= 0.0f || currentT_ >= 1.0f) {

		tSpeed_ *= -1.0f;

		currentT_ = std::clamp(currentT_, 0.0f, 1.0f);
	}

	titleLogoWorldTransform_.translation_.y = I::Lerp(8.0f, 7.5f, I::EaseInCirc(currentT_));

	UpdateWorldTransform(titleLogoWorldTransform_);

	UpdateWorldTransform(playerWorldTransform_);

	camera_->UpdateMatrix();

	if (phase_ == Phase::kFadeIn) {

		if (fade_->IsFinished()) {

			phase_ = Phase::kMain;
		}

	} else if (phase_ == Phase::kMain) {

		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {

			fade_->Start(Fade::Status::FadeOut, kFadeDuration);
			phase_ = Phase::kFadeOut;
		}

	} else {

		if (fade_->IsFinished()) {

			isFinished_ = true;
		}
	}

	fade_->Update();
}

void TitleScene::Draw() {

	KamataEngine::Model::PreDraw();

	playerModel_->Draw(playerWorldTransform_, *camera_);

	//titleLogoModel_->Draw(titleLogoWorldTransform_, *camera_);

	KamataEngine::Model::PostDraw();

	KamataEngine::Sprite::PreDraw();

	fade_->Draw();

	KamataEngine::Sprite::PostDraw();
}

TitleScene::~TitleScene() {

	delete playerModel_;
	playerModel_ = nullptr;
	delete titleLogoModel_;
	titleLogoModel_ = nullptr;
	delete camera_;
	camera_ = nullptr;
}