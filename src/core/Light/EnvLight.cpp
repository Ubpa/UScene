#include <UScene/core/Light/EnvLight.h>

#include <UScene/core/Material/Texture2D.h>

#include "detail/dynamic_reflection/EnvLight.inl"

#include <UGM/sample.h>

using namespace Ubpa;
using namespace std;

EnvLight::EnvLight(float intensity, const rgbf& color, Texture2D* texture)
	: intensity{ intensity }, color{ color }, texture{ texture }{}

rgbf EnvLight::Radiance(const pointf2& uv) const noexcept {
	if (!texture)
		return RadianceFactor();

	return texture->Sample(uv).to_rgb();
}

rgbf EnvLight::Radiance(const vecf3& dir) const noexcept {
	return Radiance(dir.normalize().cast_to<normalf>().to_sphere_texcoord());
}

tuple<rgbf, vecf3, float> EnvLight::Sample() const noexcept {
	// TODO
	vecf3 dir = uniform_on_sphere<float>().cast_to<vecf3>();
	return { Radiance(dir), dir, 1 / (4 * PI<float>) };
}

void EnvLight::OnRegist() {
	detail::dynamic_reflection::ReflRegist_EnvLight();
}
