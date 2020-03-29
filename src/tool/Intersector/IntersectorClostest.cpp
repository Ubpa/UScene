#include <UScene/tool/Intersector/IntersectorClostest.h>

#include <UScene/tool/Accel/BVH.h>
#include <UScene/core/core>

#include <UGM/UGM>

#include <stack>

using namespace Ubpa;
using namespace std;

IntersectorClostest::IntersectorClostest()
	: r{ rayf3{pointf3{}, vecf3{}} }, primitive{ nullptr }, wuv{} {
	RegistC<Square, Sphere, Triangle>();
}

const IntersectorClostest::Rst IntersectorClostest::Visit(const BVH* bvh, const rayf3& _r) const {
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
			for (auto primitiveIdx : node.PrimitiveIndices()) {
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
			if (dirIsNeg[static_cast<size_t>(node.GetAxis())]) {
				nodeIdxStack.push(firstChildIdx);
				nodeIdxStack.push(secondChildIdx); // on top
			}
			else {
				nodeIdxStack.push(secondChildIdx);
				nodeIdxStack.push(firstChildIdx); // on top
			}
		}
	}

	if (primitive) {
		/*if (vtable_is<Triangle>(primitive)) {
			const Triangle* tri = static_cast<const Triangle*>(primitive);
			rst.norm = tri->lerpNormal(wuv[0], wuv[1], wuv[2]);
			rst.tangent = tri->lerpTangent(wuv[0], wuv[1], wuv[2]);
			rst.uv = tri->lerpUV(wuv[0], wuv[1], wuv[2]);
		}*/

		rst.sobj = bvh->GetSObj(primitive);
		const auto& l2w = bvh->GetL2W(primitive);
		rst.norm = (l2w * rst.norm).normalize();
		rst.tangent = (l2w * rst.tangent).normalize();
	}

	return rst;
}

void IntersectorClostest::ImplVisit(const Square* primitive) {
	auto [isIntersect, t, hitPos] = r.intersect_std_square();
	if (isIntersect) {
		r.tmax = t;
		rst.norm = normalf{ 0, -sgn(r.dir[1]), 0 };
		rst.tangent = vecf3{ 1,0,0 };
		rst.uv = pointf2{ (hitPos[0] + 1) / 2, (1 - hitPos[1]) / 2 };
		this->primitive = primitive;
	}
}

void IntersectorClostest::ImplVisit(const Sphere* primitive) {
	auto [isIntersect, t] = r.intersect_std_sphere();
	if (isIntersect) {
		r.tmax = t;
		rst.norm = r.at(t).cast_to<normalf>().normalize();
		rst.tangent = rst.norm.to_sphere_tangent();
		rst.uv = rst.norm.to_sphere_texcoord();
		this->primitive = primitive;
	}
}

void IntersectorClostest::ImplVisit(const Triangle* primitive) {
	auto mesh = primitive->mesh;
	auto idx0 = primitive->indices[0];
	auto idx1 = primitive->indices[1];
	auto idx2 = primitive->indices[2];
	auto p0 = mesh->positions->at(idx0);
	auto p1 = mesh->positions->at(idx1);
	auto p2 = mesh->positions->at(idx2);
	auto [isIntersect, wuv, t] = r.intersect(trianglef3{ p0,p1,p2 });
	if (isIntersect) {
		r.tmax = t;
		this->primitive = primitive;
		rst.norm = primitive->lerpNormal(wuv[0], wuv[1], wuv[2]);
		rst.tangent = primitive->lerpTangent(wuv[0], wuv[1], wuv[2]);
		rst.uv = primitive->lerpUV(wuv[0], wuv[1], wuv[2]);
		this->wuv = wuv;
	}
}
