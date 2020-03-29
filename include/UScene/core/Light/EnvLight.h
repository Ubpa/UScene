#pragma once

#include "Light.h"

#include "../Resource/Image.h"

#include <UGM/rgb.h>

namespace Ubpa {
	// EnvLight's radiance = intensity * color * texture(u, v)
	struct EnvLight : Light {
		float intensity;
		rgbf color;
		Image* texture;

		EnvLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f }, Image* texture = nullptr)
			: intensity{ intensity }, color{ color }, texture{ texture }{}
	};
}
