#include <UScene/tool/Accel/BVH.h>

#include "BVHNode.h"

#include <UScene/core/Primitive/Sphere.h>
#include <UScene/core/Primitive/TriMesh.h>
#include <UScene/core/Primitive/Triangle.h>
#include <UScene/core/Primitive/Square.h>

#include <UScene/core/Cmpt/Geometry.h>
#include <UScene/core/Cmpt/Transform.h>
#include <UScene/core/Cmpt/SObjPtr.h>

#include <UScene/core/SObj.h>
#include <UScene/core/Scene.h>

#include <UDP/Visitor.h>

using namespace std;
using namespace Ubpa;

// ------------ BVHInitializer ------------

class BVH::BVHInitializer : protected Visitor<void(BVH::BVHInitializer::*)
	(BVH*, unordered_map<const Primitive*, bboxf3>&)>
{
	using Base = Visitor<void(BVH::BVHInitializer::*)
		(BVH*, unordered_map<const Primitive*, bboxf3>&)>;

public:
	BVHInitializer() { Regist<Sphere, TriMesh, Square>(); }

	static BVH::BVHInitializer& Instance() {
		static BVH::BVHInitializer instance;
		return instance;
	}

public:
	void Visit(Cmpt::Geometry* geo, Cmpt::L2W* l2w, SObj* sobj,
		BVH* bvh, unordered_map<const Primitive*, bboxf3>& p2b) {
		auto primitive = geo->primitive;
		if (!primitive) return;

		bvh->p2lw[primitive] = l2w->value;
		bvh->p2wl[primitive] = l2w->value->inverse();

		bvh->p2sobj[geo->primitive] = sobj;
		Base::Visit(geo->primitive, bvh, p2b);
	}

protected:
	void ImplVisit(Sphere* sphere, BVH* bvh, unordered_map<const Primitive*, bboxf3>& p2b) {
		const auto& l2w = bvh->GetL2W(sphere);
		bvh->primitives.push_back(sphere);
		p2b[sphere] = l2w * bboxf3{ {-1,-1,-1}, {1,1,1} };
	}

	void ImplVisit(Square* square, BVH* bvh, unordered_map<const Primitive*, bboxf3>& p2b) {
		const auto& l2w = bvh->GetL2W(square);
		bvh->primitives.push_back(square);
		p2b[square] = l2w * bboxf3{ {-1,-EPSILON<float>,-1}, {1,EPSILON<float>,1} };
	}

	void ImplVisit(TriMesh* mesh, BVH* bvh, unordered_map<const Primitive*, bboxf3>& p2b) {
		const auto& l2w = bvh->GetL2W(mesh);

		for (auto tri : mesh->triangles) {
			bvh->primitives.push_back(tri);
			p2b[tri] = l2w * tri->Box();
		}
	}
};

BVH::BVH(Scene* scene) {
	Init(scene);
}

namespace Ubpa::detail::BVH_ {
	const Primitive* get_main_primitive(const Primitive* primitive) {
		if (vtable_is<Triangle>(primitive))
			return static_cast<const Triangle*>(primitive)->mesh;
		else
			return primitive;
	}
}

const transformf& BVH::GetW2L(const Primitive* primitive) const {
	const auto target = p2wl.find(detail::BVH_::get_main_primitive(primitive));
	assert(target != p2wl.cend());

	return target->second;
}

const transformf& BVH::GetL2W(const Primitive* primitive) const {
	const auto target = p2lw.find(detail::BVH_::get_main_primitive(primitive));
	assert(target != p2lw.cend());

	return target->second;
}

const SObj* BVH::GetSObj(const Primitive* primitive) const {
	const auto target = p2sobj.find(detail::BVH_::get_main_primitive(primitive));
	assert(target != p2sobj.cend());

	return target->second;
}

const LinearBVHNode& BVH::GetNode(size_t idx) const {
	assert(idx >= 0 && idx < linearBVHNodes.size());
	return linearBVHNodes[idx];
}

const Primitive* BVH::GetPrimitive(size_t idx) const {
	assert(idx >= 0 && idx < primitives.size());
	return primitives[idx];
}

void BVH::Clear() {
	p2wl.clear();
	p2sobj.clear();
	primitives.clear();
	linearBVHNodes.clear();
}

void BVH::Init(Scene* scene) {
	Clear();
	unordered_map<const Primitive*, bboxf3> p2b;
	scene->Each([this, &p2b](Cmpt::Geometry* geo, Cmpt::L2W* l2w, Cmpt::SObjPtr* ptr) {
		BVHInitializer::Instance().Visit(geo, l2w,ptr->value,this, p2b);
	});

	BVHNode bvhRoot(p2b, primitives, 0, primitives.size());
	LinearizeBVH(&bvhRoot);
}

void BVH::LinearizeBVH(const BVHNode* bvhNode) {
	linearBVHNodes.emplace_back();
	const size_t curNodeIdx = linearBVHNodes.size() - 1;

	if (!bvhNode->IsLeaf()) {
		LinearizeBVH(bvhNode->GetL());
		linearBVHNodes[curNodeIdx].InitBranch(bvhNode->GetBBox(), static_cast<int>(linearBVHNodes.size()), bvhNode->GetAxis());
		LinearizeBVH(bvhNode->GetR());
	}
	else
		linearBVHNodes[curNodeIdx].InitLeaf(bvhNode->GetBBox(), static_cast<int>(bvhNode->GetPrimitiveOffset()), static_cast<int>(bvhNode->GetPrimitiveNum()));
}
