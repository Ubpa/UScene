#include <UScene/core/Light/AreaLight.h>

#include <UScene/core/Material/Texture2D.h>

#include "detail/dynamic_reflection/AreaLight.inl"

using namespace Ubpa;

rgbf AreaLight::Radiance(const pointf2& uv) const noexcept {
	if (!texture)
		return RadianceFactor();

	return texture->Sample(uv).to_rgb();
}

void AreaLight::OnRegister() {
	detail::dynamic_reflection::ReflRegister_AreaLight();
}
