#pragma once

#include "Light.h"

#include <UGM/rgb.h>

namespace Ubpa {
	// DirLight's radiance : intensity * color * delta(w)
	struct DirLight : Light {
		float intensity;
		rgbf color;

		rgbf Radiance() const noexcept { return intensity * color; }

		DirLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f,1.f,1.f });

		static void OnRegister();
	};
}
