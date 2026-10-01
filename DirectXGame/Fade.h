#pragma once

#include <KamataEngine.h>
#include <cstdint>

class Fade {

public:
	enum class Status { None, FadeIn, FadeOut };

private:
	Status status_ = Status::None;

	uint32_t spriteTextureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;

	float duration_ = 0.0f;
	float count_ = 0.0f;

	float deltaTime_ = 0.0f;

public:
	void Initialize();
	void Update();
	void Draw();

	void Start(const Status status, const float duration) {
		status_ = status;
		duration_ = duration;
		count_ = 0.0f;
	};

	void Stop() { status_ = Status::None; };

	bool IsFinished();

	~Fade();

};