#pragma once

#include "Primitive.h"

#include <UGM/val.h>

namespace Ubpa {
	class TriMesh;

	class Triangle : public Primitive {
	public:
		TriMesh* mesh{ nullptr };
		valu3 indices;

		Triangle() = default;
		Triangle(TriMesh* mesh, const valu3& indices) : mesh{ mesh }, indices{ indices } {}
	};
}
