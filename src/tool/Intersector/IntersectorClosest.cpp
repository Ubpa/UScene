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

IntersectorClosest::IntersectorClosest()
	: r{ rayf3{pointf3{}, vecf3{}} }, primitive{ nullptr }, wuv{} {
	RegistC<Square, Sphere, Triangle>();
}

const IntersectorClosest::Rst IntersectorClosest::Visit(const BVH* bvh, const rayf3& _r) const {
	// init
	r = _r;
	rst.sobj = nullptr;
	primitive = nullptr;

	// backup
	const pointf3 pnt = r.point;
	const vecf3 dir = r.dir;

	const bool dirIsNeg[3] = { dir[0] < 0, dir[1] < 0, dir[2] < 0 };

	stack<size_t> nodeIdxStack;
	nodeIdxStack.push(0);
	while (!nodeIdxStack.empty()) {
		const size_t nodeIdx = nodeIdxStack.top();
		nodeIdxStack.pop();
		const auto& node = bvh->GetNode(nodeIdx);
		auto [isIntersectBox, t0, t1] = r.intersect(node.GetBox());
		if (!isIntersectBox)
			continue;

		if (node.IsLeaf()) {
			for (size_t primitiveIdx : node.PrimitiveIndices()) {
				auto primitive = bvh->GetPrimitive(primitiveIdx);

				r = bvh->GetW2L(primitive) * r;
				Visit(primitive);

				// restore
				r.point = pnt;
				r.dir = dir;
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

	if (primitive) { // is intersected
		if (vtable_is<Triangle>(primitive)) { // deferred calculation
			const Triangle* tri = static_cast<const Triangle*>(primitive);
			rst.n = tri->lerpNormal(wuv[0], wuv[1], wuv[2]);
			rst.tangent = tri->lerpTangent(wuv[0], wuv[1], wuv[2]);
			rst.uv = tri->lerpUV(wuv[0], wuv[1], wuv[2]);
		}

		rst.sobj = bvh->GetSObj(primitive);
		const auto& l2w = bvh->GetL2W(primitive);
		rst.n = (l2w.transpose().inverse() * rst.n).normalize();
		vecf3 n = rst.n.cast_to<vecf3>();
		vecf3 tangent = l2w * rst.tangent;
		rst.tangent = (tangent - tangent.dot(n) * n).normalize();
		rst.pos = r.at(r.tmax);
	}

	return rst;
}

void IntersectorClosest::ImplVisit(const Square* primitive) {
	auto [isIntersect, t, hitPos] = r.intersect_std_square();

	if (isIntersect) {
		r.tmax = t;
		this->primitive = primitive;

		rst.n = normalf{ 0, -sgn(r.dir[1]), 0 };
		rst.tangent = vecf3{ 1,0,0 };
		rst.uv = pointf2{ (hitPos[0] + 1) / 2, (1 - hitPos[1]) / 2 };
	}
}

void IntersectorClosest::ImplVisit(const Sphere* primitive) {
	auto [isIntersect, t] = r.intersect_std_sphere();

	if (isIntersect) {
		r.tmax = t;
		this->primitive = primitive;

		rst.n = r.at(t).cast_to<normalf>().normalize();
		rst.tangent = rst.n.to_sphere_tangent();
		rst.uv = rst.n.to_sphere_texcoord();
	}
}

void IntersectorClosest::ImplVisit(const Triangle* primitive) {
	auto mesh = primitive->mesh;

	auto p0 = mesh->positions->at(primitive->indices[0]);
	auto p1 = mesh->positions->at(primitive->indices[1]);
	auto p2 = mesh->positions->at(primitive->indices[2]);

	auto [isIntersect, wuv, t] = r.intersect(trianglef3{ p0,p1,p2 });

	if (isIntersect) {
		r.tmax = t;
		this->primitive = primitive;

		this->wuv = wuv;
	}
}
