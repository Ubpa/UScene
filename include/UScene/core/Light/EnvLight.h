#pragma once

#include "Light.h"

#include <UGM/rgb.h>

namespace Ubpa {
	class Texture2D;

	// EnvLight's radiance = intensity * color * texture(u, v)
	struct EnvLight : Light {
		float intensity;
		rgbf color;
		Texture2D* texture;

		EnvLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f }, Texture2D* texture = nullptr)
			: intensity{ intensity }, color{ color }, texture{ texture }{}
	};
}
