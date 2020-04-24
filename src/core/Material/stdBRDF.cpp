#include <UScene/core/Material/stdBRDF.h>

#include "detail/dynamic_reflection/stdBRDF.inl"

#include <UScene/core/Material/Texture2D.h>

#include <UGM/material.h>
#include <UGM/sample.h>

using namespace Ubpa;
using namespace std;

void stdBRDF::OnRegist() {
	detail::dynamic_reflection::ReflRegist_stdBRDF();
}

const rgbf stdBRDF::Albedo(const pointf2& uv) const {
	if (!albedo_texture)
		return albedo_factor;

	return albedo_factor * albedo_texture->Sample(uv).to_rgb();
}

const float stdBRDF::Roughness(const pointf2& uv) const {
	if (!roughness_texture)
		return roughness_factor;

	return roughness_factor * roughness_texture->Sample(uv)[0];
}

const float stdBRDF::Metalness(const pointf2& uv) const {
	if (!metalness_texture)
		return metalness_factor;

	return metalness_factor * metalness_texture->Sample(uv)[0];
}

const normalf stdBRDF::Normal(const pointf2& uv) const {
	if (!normal_map)
		return { 0,0,1 };

	rgbf normal_map_pixel = normal_map->Sample(uv).to_rgb();
	return (normal_map_pixel * 2 - rgbf{ 1.f }).cast_to<normalf>().normalize();
}

rgbf stdBRDF::F0(float metalness, const rgbf& albedo) {
	return rgbf::lerp(rgbf{ 0.04f }, albedo, metalness);
}

rgbf stdBRDF::Fresnel(const rgbf& f0, float cos_theta) {
	return f0 + (rgbf{ 1.f } -f0) * pow5(1 - cos_theta);
}

float stdBRDF::GGX_D(float alpha, const svecf& wm) {
	return Ubpa::GGX_D(alpha, wm);
}

float stdBRDF::GGX_G(float alpha, const svecf& wi, const svecf& wo, const svecf& wm) {
	return Ubpa::GGX_G(alpha, wi, wo, wm);
}

rgbf stdBRDF::BRDF(const rgbf& albedo, float metalness, float roughness, const svecf& wi, const svecf& wo) {
	svecf wm = (wi + wo).normalize();
	float alpha = Alpha(roughness);
	rgbf f0 = F0(metalness, albedo);
	rgbf F = Fresnel(f0, wi.dot(wm));
	rgbf D = GGX_D(alpha, wm);
	rgbf G = GGX_G(alpha, wi, wo, wm);
	rgbf diffuse = (1 - metalness) / PI<float> * (rgbf{ 1.f }-F) * albedo;
	rgbf specular = F * D * G / (4 * wi.cos_stheta() * wo.cos_stheta());
	return diffuse + specular;
}

float stdBRDF::SampleDiffusePD(const rgbf& albedo, float metalness, float roughness, const svecf& wo) {
	float alpha = Alpha(roughness);
	rgbf f0 = F0(metalness, albedo);
	constexpr svecf predict_wm = svecf{ 0,0,1 };
	const svecf predict_wi = wo.reflect();
	rgbf predict_Fresnel = rgbf::mid(rgbf{ 1.f }, f0);
	float predict_GGX_D = 1 / (PI<float> * pow2(alpha)); // GGX_D(alpha, predict_wm);
	float predict_GGX_G = GGX_G(alpha, predict_wi, wo, predict_wm);
	float predict_diffuse = ((rgbf{ 1.f }-predict_Fresnel) / 2 * albedo).gray() * (1 - metalness) / PI<float>;
	float predict_specular = predict_Fresnel.gray() * predict_GGX_D * predict_GGX_G / (4 * wo.cos_stheta());
	float p_diffuse = predict_diffuse / (predict_diffuse + predict_specular);
	// 1 - p_diffuse
	// float p_specular = predict_specular / (predict_diffuse + predict_specular);

	return p_diffuse;
}

float stdBRDF::PDF(const rgbf& albedo, float metalness, float roughness, const svecf& wi, const svecf& wo) {
	float alpha = Alpha(roughness);

	float p_diffuse = SampleDiffusePD(albedo, metalness, roughness, wo);

	svecf wm = (wi + wo).normalize();
	float dwm_dwi = 1.f / (4 * std::abs(wi.dot(wm)));
	float pd_specular = GGX_D(alpha, wm) * dwm_dwi;
	float pd_diffuse = wi.cos_stheta() / PI<float>;

	float pd = p_diffuse * pd_diffuse + (1 - p_diffuse) * pd_specular;

	return pd;
}

tuple<svecf, float> stdBRDF::Sample(const rgbf& albedo, float metalness, float roughness, const svecf& wo) {
	float alpha = Alpha(roughness);

	float p_diffuse = SampleDiffusePD(albedo, metalness, roughness, wo);

	svecf wi;
	if (rand01<float>() < p_diffuse) {
		// diffuse
		wi = cos_weighted_on_hemisphere<float>().cast_to<svecf>();
	}
	else {
		// specular
		wi = sample_GGX_D(alpha);
	}
	svecf wm = (wi + wo).normalize();

	float pd_diffuse = wi.cos_stheta() / PI<float>;
	float pd_specular = GGX_D(alpha, wm) / (4 * std::abs(wi.dot(wm)));
	
	float pd = p_diffuse * pd_diffuse + (1 - p_diffuse) * pd_specular;

	return { wm, pd };
}
