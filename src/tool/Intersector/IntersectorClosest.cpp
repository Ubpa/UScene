#include <UScene/tool/Intersector/IntersectorClosest.h>

#include <UScene/tool/Accel/BVH.h>
#include <UScene/core/Primitive/Square.h>
#include <UScene/core/Primitive/Triangle.h>
#include <UScene/core/Primitive/Sphere.h>
#include <UScene/core/Primitive/TriMesh.h>

#include <UGM/UGM.h>

#include <stack>

using namespace Ubpa;
using namespace std;

IntersectorClosest::IntersectorClosest() {
	auto intersect_square = [](const Square* square, Rst& rst, Temp& tmp) {
		auto [isIntersect, t, hitPos] = tmp.r.intersect_std_square();

		if (isIntersect) {
			tmp.r.tmax = t;

			rst.primitive = square;
			rst.n = normalf{ 0, -sgn(tmp.r.dir[1]), 0 };
			rst.tangent = vecf3{ 1,0,0 };
			rst.uv = pointf2{ hitPos[0] / Square::side_length + 0.5f, 0.5f - hitPos[1] / Square::side_length };
		}
	};

	auto intersect_sphere = [](const Sphere* primitive, Rst& rst, Temp& tmp) {
		auto [isIntersect, t] = tmp.r.intersect_std_sphere();

		if (isIntersect) {
			tmp.r.tmax = t;

			rst.primitive = primitive;
			rst.n = tmp.r.at(t).cast_to<normalf>().normalize();
			rst.tangent = rst.n.to_sphere_tangent();
			rst.uv = rst.n.to_sphere_texcoord();
		}
	};

	auto intersect_tri = [](const Triangle* primitive, Rst& rst, Temp& tmp) {
		auto mesh = primitive->mesh;

		auto p0 = mesh->positions->at(primitive->indices[0]);
		auto p1 = mesh->positions->at(primitive->indices[1]);
		auto p2 = mesh->positions->at(primitive->indices[2]);

		auto [isIntersect, wuv, t] = tmp.r.intersect(trianglef3{ p0,p1,p2 });

		if (isIntersect) {
			tmp.r.tmax = t;
			tmp.wuv = wuv;

			rst.primitive = primitive;
		}
	};

	visitor.Regist(intersect_square, intersect_sphere, intersect_tri);
}

const IntersectorClosest::Rst IntersectorClosest::Visit(const BVH* bvh, const rayf3& r_) const {
	// init
	Temp tmp(r_);
	Rst rst;

	// backup
	const pointf3 pnt = tmp.r.point;
	const vecf3 dir = tmp.r.dir;

	const bool dirIsNeg[3] = { dir[0] < 0, dir[1] < 0, dir[2] < 0 };

	stack<size_t> nodeIdxStack;
	nodeIdxStack.push(0);
	while (!nodeIdxStack.empty()) {
		const size_t nodeIdx = nodeIdxStack.top();
		nodeIdxStack.pop();
		const auto& node = bvh->GetNode(nodeIdx);
		auto [isIntersectBox, t0, t1] = tmp.r.intersect(node.GetBox());
		if (!isIntersectBox)
			continue;

		if (node.IsLeaf()) {
			for (size_t primitiveIdx : node.PrimitiveIndices()) {
				auto primitive = bvh->GetPrimitive(primitiveIdx);

				tmp.r = bvh->GetW2L(primitive) * tmp.r;
				visitor.Visit(primitive, rst, tmp);

				// restore
				tmp.r.point = pnt;
				tmp.r.dir = dir;
			}
		}
		else {
			size_t firstChildIdx = LinearBVHNode::FirstChildIdx(nodeIdx);
			size_t secondChildIdx = node.GetSecondChildIdx();
			if (dirIsNeg[static_cast<size_t>(node.GetAxis())])
				std::swap(firstChildIdx, secondChildIdx);
			nodeIdxStack.push(secondChildIdx);
			nodeIdxStack.push(firstChildIdx); // on top
		}
	}

	if (rst.primitive) { // is intersected
		if (vtable_is<Triangle>(rst.primitive)) { // deferred calculation
			const Triangle* tri = static_cast<const Triangle*>(rst.primitive);
			rst.n = tri->LerpNormal(tmp.wuv[0], tmp.wuv[1], tmp.wuv[2]);
			rst.tangent = tri->LerpTangent(tmp.wuv[0], tmp.wuv[1], tmp.wuv[2]);
			rst.uv = tri->LerpUV(tmp.wuv[0], tmp.wuv[1], tmp.wuv[2]);
		}

		rst.sobj = bvh->GetSObj(rst.primitive);
		const auto& l2w = bvh->GetL2W(rst.primitive);
		rst.n = (l2w.transpose().inverse() * rst.n).normalize();
		vecf3 n = rst.n.cast_to<vecf3>();
		vecf3 tangent = l2w * rst.tangent;
		rst.tangent = (tangent - tangent.dot(n) * n).normalize();
		rst.pos = tmp.r.at(tmp.r.tmax);
	}

	return rst;
}
