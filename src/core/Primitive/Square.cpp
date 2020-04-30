#pragma once

#include <UScene/core/Primitive/Square.h>

#include "detail/dynamic_reflection/Square.inl"

using namespace Ubpa;

void Square::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Square();
}
