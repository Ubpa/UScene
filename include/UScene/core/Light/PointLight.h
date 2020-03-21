#pragma once

#include "Light.h"

namespace Ubpa {
	struct PointLight : Light {
		// point light's luminance : intensity * color * delta(A)
		float intensity;
		rgbf color;

		PointLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f })
			: intensity(intensity), color(color) {}
	};
}
