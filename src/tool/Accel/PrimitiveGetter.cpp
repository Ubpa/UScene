#include <UScene/tool/Accel/detail/PrimitiveGetter.h>

#include <UScene/core/Primitive/Plane.h>
#include <UScene/core/Primitive/Sphere.h>
#include <UScene/core/Primitive/Triangle.h>
#include <UScene/core/Primitive/TriMesh.h>

using namespace Ubpa;

detail::Accel_::PrimitiveGetter::PrimitiveGetter() {
	Regist<Plane, Sphere, TriMesh>();

	Regist([this](Triangle* tri) {
		rst = tri->mesh;
	});
}

Primitive* detail::Accel_::PrimitiveGetter::Visit(Primitive* primitive) const {
	RawPtrVisitor<PrimitiveGetter, Primitive>::Visit(primitive);
	return rst;
}
