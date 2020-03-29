#pragma once

#include "Light.h"

#include <UGM/rgb.h>

namespace Ubpa {
	// PointLight's radiance : intensity * color * delta(A)
	struct PointLight : Light {
		float intensity;
		rgbf color;

		PointLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f })
			: intensity(intensity), color(color) {}
	};
}
