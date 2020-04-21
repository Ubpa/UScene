#pragma once

#include "Material.h"

#include <UGM/rgb.h>

namespace Ubpa {
	class Texture2D;

	// basic material
	struct stdBRDF : Material {
		rgbf albedo_factor;
		Texture2D* albedo_texture;

		float roughness_factor;
		Texture2D* roughness_texture;

		float metalness_factor;
		Texture2D* metalness_texture;

		Texture2D* normal_map;

		stdBRDF(const rgbf& albedo_factor = rgbf{ 1.f },
			Texture2D* albedo_texture = nullptr,
			float roughness_factor = 1.f,
			Texture2D* roughness_texture = nullptr,
			float metalness_factor = 1.f,
			Texture2D* metalness_texture = nullptr,
			Texture2D* normal_map = nullptr ) noexcept :
			albedo_factor{ albedo_factor },
			albedo_texture{ albedo_texture },
			roughness_factor{ roughness_factor },
			roughness_texture{ roughness_texture },
			metalness_factor{ metalness_factor },
			metalness_texture{ metalness_texture },
			normal_map{ normal_map }
			{}

		static void OnRegist();
	};
}
