#include <UScene/core/Primitive/Triangle.h>

#include <UScene/core/Primitive/TriMesh.h>

#include "detail/dynamic_reflection/Triangle.inl"

using namespace Ubpa;
using namespace std;

const pointf2 Triangle::LerpUV(float w, float u, float v) const {
	assert(mesh != nullptr);
	const auto& texcoords = mesh->texcoords.get();
	return pointf2::combine(
		array{ texcoords[indices[0]],texcoords[indices[1]],texcoords[indices[2]] }
	, array{ w,u,v });
}

const pointf3 Triangle::LerpPosition(float w, float u, float v) const {
	assert(mesh != nullptr);
	const auto& positions = mesh->positions.get();
	return pointf3::combine(
		array{ positions[indices[0]],positions[indices[1]],positions[indices[2]] }
	, array{ w,u,v });
}

const normalf Triangle::LerpNormal(float w, float u, float v) const {
	assert(mesh != nullptr);
	const auto& normals = mesh->normals.get();
	normalf n = w * normals[indices[0]] + u * normals[indices[1]] + v * normals[indices[2]];
	return n.normalize();
}

const vecf3 Triangle::LerpTangent(float w, float u, float v) const {
	assert(mesh != nullptr);
	const auto& tangents = mesh->tangents.get();
	vecf3 tangent = w * tangents[indices[0]] + u * tangents[indices[1]] + v * tangents[indices[2]];
	return tangent.normalize();
}

const bboxf3 Triangle::Box() const {
	assert(mesh != nullptr);
	const auto& positions = mesh->positions.get();
	bboxf3 b = bboxf3::minmax(positions[indices[0]], positions[indices[1]]).combine(positions[indices[2]]);
	b.minP() -= EPSILON<float>;
	b.maxP() += EPSILON<float>;
	return b;
}

void Triangle::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Triangle();
}
