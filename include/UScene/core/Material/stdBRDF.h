#pragma once

#include "Material.h"

#include <UGM/rgb.h>
#include <UGM/point.h>
#include <UGM/normal.h>
#include <UGM/svec.h>

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

		const rgbf Albedo(const pointf2& uv) const;
		const float Roughness(const pointf2& uv) const;
		const float Metalness(const pointf2& uv) const;
		const normalf Normal(const pointf2& uv) const;
		static float Alpha(float roughness) noexcept { return roughness * roughness; }

		static rgbf F0(float metalness, const rgbf& albedo);
		static rgbf Fresnel(const rgbf& F0, float cos_theta);
		static float GGX_D(float alpha, const svecf& wm);
		static float GGX_G(float alpha, const svecf& wi, const svecf& wo, const svecf& wm);
		static rgbf BRDF(const rgbf& albedo, float metalness, float roughness, const svecf& wi, const svecf& wo);
		static float PDF(const rgbf& albedo, float metalness, float roughness, const svecf& wi, const svecf& wo);
		// wi, pdf
		static std::tuple<svecf, float> Sample(const rgbf& albedo, float metalness, float roughness, const svecf& wo);

		static void OnRegist();
	};
}
