#pragma once
#include <KamataEngine.h>

class KeMainCamera {

public:
	KamataEngine::Camera* camera_ = nullptr;

	void Initialize() {
		camera_ = new KamataEngine::Camera();
		camera_->Initialize();
	}

	static KeMainCamera* GetInstance() {

		static KeMainCamera instance;

		return &instance;
	}

	~KeMainCamera() {
		if (camera_) {
			delete camera_;
			camera_ = nullptr;
		}
	}
};