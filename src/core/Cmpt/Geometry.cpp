#include <UScene/core/Cmpt/Geometry.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Geometry.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Geometry::OnRegist() {
	detail::dynamic_reflection::ReflRegist_Geometry();
}

Cmpt::Geometry::Geometry() = default;

Cmpt::Geometry::~Geometry() { delete primitive; }

Cmpt::Geometry::Geometry(Geometry&& geo) noexcept
	: primitive{ geo.primitive }
{
	geo.primitive.val = nullptr;
}

void Cmpt::Geometry::SetPrimitive(Primitive* primitive) {
	delete this->primitive;
	this->primitive.val = primitive;
}
