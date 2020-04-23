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

float stdBRDF::PDF(const rgbf& albedo, float metalness, float roughness, const svecf& wi, const svecf& wo) {
	// TODO
	return 1 / (2 * PI<float>);
}

tuple<svecf, float> stdBRDF::Sample(const rgbf& albedo, float metalness, float roughness, const svecf& wo) {
	// TODO
	svec wi = cos_weighted_on_hemisphere<float>().cast_to<svecf>();
	float pdf = 1 / (2 * PI<float>);
	return { wi, pdf };
}
