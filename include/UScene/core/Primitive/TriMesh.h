#pragma once

#include "Primitive.h"
#include "Triangle.h"

#include <UGM/UGM.h>
#include <UDP/Basic/Read.h>
#include <UBL/Pool.h>

#include <vector>
#include <string>

namespace Ubpa {
	class [[serialize_kernel("path")]] TriMesh : public Primitive {
	public:
		[[not_UI]]
		Read<TriMesh, std::vector<valu3>> indices; // unsigned is for OpenGL
		[[not_UI]]
		Read<TriMesh, std::vector<pointf3>> positions;
		[[not_UI]]
		Read<TriMesh, std::vector<pointf2>> texcoords;
		[[not_UI]]
		Read<TriMesh, std::vector<normalf>> normals;
		[[not_UI]]
		Read<TriMesh, std::vector<vecf3>> tangents;
		[[not_serialize, not_UI]]
		Read<TriMesh, std::vector<const Triangle*>> triangles;
		
		Read<TriMesh, std::string> path;

		TriMesh() = default;

		TriMesh(const std::vector<valu3>& indices,
			const std::vector<pointf3>& positions,
			const std::vector<pointf2>& texcoords = std::vector<pointf2>(),
			const std::vector<normalf>& normals = std::vector<normalf>(),
			const std::vector<vecf3>& tangents = std::vector<vecf3>());

		enum class Type { Cube, Sphere, Square };
		TriMesh(Type type);

		TriMesh(const std::string& path);

		// center : (0, 0, 0), side length: 2
		void InitCubeMesh();
		// 50 x 50 grid
		void InitSphereMesh();
		// center : (0, 0, 0), side length: 2, normal: (0, 1, 0)
		void InitSquareMesh();

		bool Init(const std::vector<valu3>& indices,
			const std::vector<pointf3>& positions,
			const std::vector<pointf2>& texcoords = std::vector<pointf2>(),
			const std::vector<normalf>& normals = std::vector<normalf>(),
			const std::vector<vecf3>& tangents = std::vector<vecf3>());

		bool Init(const std::string& path);

		// .obj
		bool SetAndSave(const std::string& path);

		bool Empty() const noexcept;

		void Clear();

		void GenNormals();
		void GenTangents();

		static void OnRegister();

	private:
		Pool<Triangle> triPool;
	};
}
