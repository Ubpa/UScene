#include <UScene/tool/Accel/BVH.h>

#include "BVHNode.h"

#include <UScene/core/Primitive/Sphere.h>
#include <UScene/core/Primitive/TriMesh.h>
#include <UScene/core/Primitive/Triangle.h>
#include <UScene/core/Primitive/Plane.h>

#include <UScene/core/Cmpt/Geometry.h>
#include <UScene/core/Cmpt/Transform.h>

#include <UScene/core/SObj.h>
#include <UScene/core/Scene.h>

#include <UDP/Visitor/Visitor.h>

using namespace std;
using namespace Ubpa;

// ------------ BVHInitializer ------------

class BVH::BVHInitializer : public RawPtrVisitor<BVH::BVHInitializer, Primitive> {
public:
	BVHInitializer(BVH* holder) : holder(holder) {
		Regist<Sphere, TriMesh, Plane>();
	}
	virtual ~BVHInitializer() = default;

public:
	unordered_map<Primitive*, bboxf3> p2b;

public:
	using RawPtrVisitor<BVH::BVHInitializer, Primitive>::Visit;
	void Visit(Cmpt::Geometry* geo) {
		auto primitive = geo->primitive;
		if (!primitive)
			return;

		auto l2w = geo->sobj->GetOrAttach<Cmpt::Transform>()->GetLocalToWorldMatrix();
		holder->p2lw[primitive] = l2w;
		holder->p2wl[primitive] = l2w.inverse();

		holder->p2sobj[geo->primitive] = geo->sobj.get();
		Visit(geo->primitive);
	}

protected:
	void ImplVisit(Sphere* sphere) {
		const auto l2w = holder->GetL2W(sphere);
		holder->primitives.push_back(sphere);
		p2b[sphere] = l2w * bboxf3{ {-1,-1,-1}, {1,1,1} };
	}

	void ImplVisit(Plane* plane) {
		const auto l2w = holder->GetL2W(plane);
		holder->primitives.push_back(plane);
		p2b[plane] = l2w * bboxf3{ {-1,-1,-1 - EPSILON<float>}, {1,1,1 + EPSILON<float>} };
	}

	void ImplVisit(TriMesh* mesh) {
		const auto l2w = holder->GetL2W(mesh);
		holder->primitives.push_back(mesh);

		bboxf3 meshbox;
		for (const auto& pos : mesh->positions.get())
			meshbox.combine_to_self(pos);
		p2b[mesh] = l2w * meshbox;

		for (const auto& tri : mesh->indices.get()) {
			auto triPrimitive = new Triangle(mesh, tri);
			holder->need_delete_primitives.push_back(triPrimitive);
			holder->primitives.push_back(triPrimitive);
			bboxf3 tribox;
			for (auto idx : tri)
				tribox.combine_to_self(mesh->positions->at(idx));
			p2b[triPrimitive] = l2w * tribox;
		}
	}

private:
	BVH* holder;
};

BVH::BVH(Scene* scene) {
	Init(scene);
}

BVH::~BVH() {
	// triangles
	for (auto primitive : need_delete_primitives)
		delete primitive;
}

const transformf& BVH::GetW2L(Primitive* primitive) const {
	const auto target = p2wl.find(primitiveGetter.Visit(primitive));
	assert(target != p2wl.cend());

	return target->second;
}

const transformf& BVH::GetL2W(Primitive* primitive) const {
	const auto target = p2lw.find(primitiveGetter.Visit(primitive));
	assert(target != p2lw.cend());

	return target->second;
}

const SObj* BVH::GetSObj(Primitive* primitive) const {
	const auto target = p2sobj.find(primitiveGetter.Visit(primitive));
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
	BVHInitializer initializer(this);
	scene->Each([&initializer](Cmpt::Geometry* geo) {
		initializer.Visit(geo);
	});

	BVHNode bvhRoot(initializer.p2b, primitives, 0, primitives.size());
	LinearizeBVH(&bvhRoot);
}

void BVH::LinearizeBVH(BVHNode* bvhNode) {
	linearBVHNodes.push_back(LinearBVHNode());
	const auto curNodeIdx = linearBVHNodes.size() - 1;

	if (!bvhNode->IsLeaf()) {
		LinearizeBVH(bvhNode->GetL());
		linearBVHNodes[curNodeIdx].InitBranch(bvhNode->GetBBox(), static_cast<int>(linearBVHNodes.size()), bvhNode->GetAxis());
		LinearizeBVH(bvhNode->GetR());
	}
	else
		linearBVHNodes[curNodeIdx].InitLeaf(bvhNode->GetBBox(), static_cast<int>(bvhNode->GetPrimitiveOffset()), static_cast<int>(bvhNode->GetPrimitiveNum()));
}
