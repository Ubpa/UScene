#pragma once

#include "Primitive.h"

#include <UGM/UGM>
#include <UDP/Basic/Read.h>

#include <vector>

namespace Ubpa {
	class TriMesh : public Primitive {
	public:
		Read<TriMesh, std::vector<valu3>> indices; // unsigned is for OpenGL
		Read<TriMesh, std::vector<pointf3>> positions;
		Read<TriMesh, std::vector<pointf2>> texcoords;
		Read<TriMesh, std::vector<normalf>> normals;
		Read<TriMesh, std::vector<vecf3>> tangents;

		TriMesh() = default;

		TriMesh(const std::vector<valu3>& indices,
			const std::vector<pointf3>& positions,
			const std::vector<pointf2>& texcoords = std::vector<pointf2>(),
			const std::vector<normalf>& normals = std::vector<normalf>(),
			const std::vector<vecf3>& tangents = std::vector<vecf3>());

			
		enum class Type { Cube, Sphere, Plane };
		TriMesh(Type type);

		// center : (0, 0, 0), side length: 2
		void InitCubeMesh();
		// 50 x 50 grid
		void InitSphereMesh();
		// center : (0, 0, 0), side length: 2, normal: (0, 1, 0)
		void InitPlaneMesh();

		bool Init(const std::vector<valu3>& indices,
			const std::vector<pointf3>& positions,
			const std::vector<pointf2>& texcoords = std::vector<pointf2>(),
			const std::vector<normalf>& normals = std::vector<normalf>(),
			const std::vector<vecf3>& tangents = std::vector<vecf3>());

		bool Empty() const noexcept;

		void Clear();

		void GenNormals();
		void GenTangents();
	};
}
