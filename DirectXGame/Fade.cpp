#include "Fade.h"
#include "./Time/DeltaTime.h"
#include "Easing.h"
#include "./Math/Lerp.h"

namespace {

namespace I = Atrum::Interpolation;
namespace A = Atrum;

}

void Fade::Initialize() {

	spriteTextureHandle_ = KamataEngine::TextureManager::GetInstance()->Load("./Resources/white1x1.png");
	sprite_ = KamataEngine::Sprite::Create(spriteTextureHandle_, {});
	sprite_->SetSize(KamataEngine::Vector2{1280.0f, 720.0f});
	sprite_->SetColor(KamataEngine::Vector4{0.0f, 0.0f, 0.0f, 1.0f});
}

void Fade::Update() {

	if (status_ == Status::None) {
		return;
	}

	deltaTime_ = A::FrameDeltaTime::GetInstance()->GetDeltaTime();

	count_ += deltaTime_;

	if (count_ >= duration_) {

		count_ = duration_;

		status_ = Status::None;

		return;
	}

	if (status_ == Status::FadeOut) {

		sprite_->SetColor(KamataEngine::Vector4{0.0f, 0.0f, 0.0f, I::Lerp(0.0f, 1.0f, I::EaseInCirc(count_ / duration_))});
	} else if (status_ == Status::FadeIn) {

		sprite_->SetColor(KamataEngine::Vector4{0.0f, 0.0f, 0.0f, I::Lerp(1.0f, 0.0f, I::EaseInCirc(count_ / duration_))});
	}
}

void Fade::Draw() {

	if (sprite_->GetColor().w <= 0.0f) {
		return;
	}

	sprite_->Draw();
}

bool Fade::IsFinished() {

	if (status_ == Status::None) {

		return true;
	}

	return (count_ >= duration_);
}

Fade::~Fade() {

	delete sprite_;
	sprite_ = nullptr;
}