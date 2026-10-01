#pragma once

#include <cmath>

namespace Atrum::Interpolation {

inline float EaseInQuad(const float& t) { return t * t; }

inline float EaseOutQuad(const float& t) { return 1 - (1 - t) * (1 - t); }

inline float EaseInOutQuad(const float& t) {

	if (t < 0.5f) {

		return 2.0f * t * t;

	} else {

		return 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) / 2.0f;
	}
}

inline float EaseInQuart(const float& t) { return t * t * t * t; }

inline float EaseOutQuart(const float& t) { return 1 - (1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t); }

inline float EaseInOutQuart(const float& t) {

	if (t < 0.5f) {

		return 8 * t * t * t * t;

	} else {

		return 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) / 2.0f;
	}
}

inline float EaseInCirc(const float& t) { return 1.0f - sqrtf(1.0f - t * t); }

inline float EaseOutCirc(const float& t) { return sqrtf(1.0f - (t - 1.0f) * (t - 1.0f)); }

inline float EaseInOutCirc(const float& t) {

	if (t < 0.5f) {

		return (1 - sqrtf(1 - (2.0f * t) * (2.0f * t))) / 2.0f;

	} else {

		return (sqrtf(1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f)) + 1.0f) / 2.0f;
	}
}

inline float EaseInBack(const float& t) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

inline float EaseOutBack(const float& t) {

	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) + c1 * (t - 1.0f) * (t - 1.0f);
}

} // namespace Atrum::Interpolation