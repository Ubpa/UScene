#pragma once

#include <UScene/core/Light/DirLight.h>

#include "detail/dynamic_reflection/DirLight.inl"

using namespace Ubpa;

DirLight::DirLight(float intensity, const rgbf& color)
	: intensity(intensity), color(color) {}

void DirLight::OnRegister() {
	detail::dynamic_reflection::ReflRegister_DirLight();
}
