#pragma once

#include "Light.h"

#include <UGM/rgb.h>

namespace Ubpa {
	// PointLight's radiance : intensity * color * delta(A)
	struct PointLight : Light {
		[[range("0,")]]
		float intensity;
		rgbf color;

		rgbf RadianceFactor() const noexcept { return intensity * color; }

		PointLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f,1.f,1.f });

		static void OnRegister();
	};
}
