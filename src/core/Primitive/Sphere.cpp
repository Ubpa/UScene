#pragma once

#include <UScene/core/Primitive/Sphere.h>

#include "detail/dynamic_reflection/Sphere.inl"

using namespace Ubpa;

void Sphere::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Sphere();
}
