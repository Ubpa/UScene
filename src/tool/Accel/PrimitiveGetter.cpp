#include <UScene/tool/Accel/detail/PrimitiveGetter.h>

#include <UScene/core/Primitive/Square.h>
#include <UScene/core/Primitive/Sphere.h>
#include <UScene/core/Primitive/Triangle.h>
#include <UScene/core/Primitive/TriMesh.h>

using namespace Ubpa;

detail::Accel_::PrimitiveGetter::PrimitiveGetter() {
	RegistC<Square, Sphere, TriMesh>();

	Regist([this](const Triangle* tri) {
		rst = tri->mesh;
	});
}

const Primitive* detail::Accel_::PrimitiveGetter::Visit(const Primitive* primitive) const {
	RawPtrVisitor<PrimitiveGetter, Primitive>::Visit(primitive);
	return rst;
}
