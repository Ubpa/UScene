#pragma once

#include "Primitive.h"

#include <UGM/val.h>
#include <UGM/bbox.h>

namespace Ubpa {
	class TriMesh;

	class Triangle : public Primitive {
	public:
		TriMesh* mesh{ nullptr };
		valu3 indices;

		Triangle() = default;
		Triangle(TriMesh* mesh, const valu3& indices) : mesh{ mesh }, indices{ indices } {}

		const pointf2 LerpUV(float w, float u, float v) const;
		const pointf3 LerpPosition(float w, float u, float v) const;
		const normalf LerpNormal(float w, float u, float v) const;
		const vecf3 LerpTangent(float w, float u, float v) const;
		const bboxf3 Box() const;

		static void OnRegist();
	};
}
