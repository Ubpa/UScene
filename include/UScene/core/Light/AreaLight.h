#pragma once

#include "Light.h"

#include "../Resource/Image.h"

#include <UGM/rgb.h>

namespace Ubpa {
	// AreaLight's radiance = intensity * color * texture(u, v)
	struct AreaLight : Light {
		float intensity;
		rgbf color;
		Image* texture;

		AreaLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f }, Image* texture = nullptr)
			: intensity{ intensity }, color{ color }, texture{ texture }{}
	};
}
