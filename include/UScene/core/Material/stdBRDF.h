#pragma once

#include "Material.h"

#include "Image.h"

#include <UGM/rgb.h>

namespace Ubpa {
	struct stdBRDF : Material {
		rgbf albedo_factor;
		Image* albedo_texture;

		float roughness_factor;
		Image* roughness_texture;

		float metalness_factor;
		Image* metalness_texture;

		Image* normal_map;

		stdBRDF(const rgbf& albedo_factor = rgbf{ 1.f },
			Image* albedo_texture = nullptr,
			float roughness_factor = 1.f,
			Image* roughness_texture = nullptr,
			float metalness_factor = 1.f,
			Image* metalness_texture = nullptr,
			Image* normal_map = nullptr ) noexcept :
			albedo_factor{ albedo_factor },
			albedo_texture{ albedo_texture },
			roughness_factor{ roughness_factor },
			roughness_texture{ roughness_texture },
			metalness_factor{ metalness_factor },
			metalness_texture{ metalness_texture },
			normal_map{ normal_map }
			{}
	};
}
