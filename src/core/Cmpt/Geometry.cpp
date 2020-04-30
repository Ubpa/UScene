#include <UScene/core/Cmpt/Geometry.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Geometry.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Geometry::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Geometry();
}

Cmpt::Geometry::Geometry() = default;

Cmpt::Geometry::~Geometry() { delete primitive; }

Cmpt::Geometry::Geometry(Geometry&& geo) noexcept
	: primitive{ geo.primitive }
{
	geo.primitive= nullptr;
}

void Cmpt::Geometry::SetPrimitive(Primitive* primitive) {
	delete this->primitive;
	this->primitive= primitive;
}
