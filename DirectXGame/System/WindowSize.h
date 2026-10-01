#pragma once

namespace Atrum {

class WindowSize final {

private:
	float width_ = 1280.0f;
	float height_ = 720.0f;

	WindowSize() = default;
	~WindowSize() = default;

public:
	float GetWidth() { return width_; }
	float GetHeight() { return height_; }

	static WindowSize* GetInstance() {

		static WindowSize instance;

		return &instance;
	}

	WindowSize operator=(const WindowSize& source) = delete;
	WindowSize(const WindowSize& source) = delete;
};

} // namespace Atrum