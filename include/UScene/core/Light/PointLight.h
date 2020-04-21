#pragma once

#include "Light.h"

#include <UGM/rgb.h>

namespace Ubpa {
	// PointLight's radiance : intensity * color * delta(A)
	struct PointLight : Light {
		float intensity;
		rgbf color;

		rgbf radiance_factor() const noexcept { return intensity * color; }

		PointLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f,1.f,1.f });

		static void OnRegist();
	};
}
