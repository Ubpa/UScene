#pragma once

#include "Light.h"

#include <UGM/rgb.h>
#include <UGM/point.h>
#include <UGM/vec.h>

namespace Ubpa {
	class Texture2D;

	// EnvLight's radiance = intensity * color * texture(u, v)
	struct EnvLight : Light {
		float intensity;
		rgbf color;
		Texture2D* texture;

		rgbf RadianceFactor() const noexcept { return intensity * color; }
		rgbf Radiance(const pointf2& uv) const noexcept;
		rgbf Radiance(const vecf3& dir) const noexcept;
		// radiance, direction, pdf
		std::tuple<rgbf, vecf3, float> Sample() const noexcept;

		EnvLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f,1.f,1.f }, Texture2D* texture = nullptr);

		static void OnRegist();
	};
}
