#pragma once

#include <UScene/core/Light/PointLight.h>

#include "detail/dynamic_reflection/PointLight.inl"

using namespace Ubpa;

PointLight::PointLight(float intensity, const rgbf& color)
	: intensity(intensity), color(color) {}

void PointLight::OnRegister() {
	detail::dynamic_reflection::ReflRegister_PointLight();
}
