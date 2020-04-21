#pragma once

#include <UScene/core/Primitive/Sphere.h>

#include "detail/dynamic_reflection/Sphere.inl"

using namespace Ubpa;

void Sphere::OnRegist() {
	detail::dynamic_reflection::ReflRegist_Sphere();
}
