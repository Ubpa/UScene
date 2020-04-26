#include <UScene/tool/Intersector/IntersectorVisibility.h>

#include <UScene/core.h>
#include <UScene/tool/Accel/BVH.h>

#include <UGM/UGM.h>

#include <stack>

using namespace Ubpa;
using namespace std;

IntersectorVisibility::IntersectorVisibility() {
	auto intersect_square = [](const Square*, const rayf3& r) {
		auto [isIntersect, t, hitPos] = r.intersect_std_square();
		return isIntersect;
	};

	auto intersect_sphere = [](const Sphere*, const rayf3& r) {
		auto [isIntersect, t] = r.intersect_std_sphere();
		return isIntersect;
	};

	auto intersect_tri = [](const Triangle* primitive, const rayf3& r) {
		auto mesh = primitive->mesh;
		auto p0 = mesh->positions->at(primitive->indices[0]);
		auto p1 = mesh->positions->at(primitive->indices[1]);
		auto p2 = mesh->positions->at(primitive->indices[2]);
		auto [isIntersect, wuv, t] = r.intersect(trianglef3{ p0,p1,p2 });
		return isIntersect;
	};

	visitor.Regist(intersect_square, intersect_sphere, intersect_tri);
}

bool IntersectorVisibility::Visit(const BVH* bvh, rayf3 r) const {
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
				if (visitor.Visit(primitive, r))
					return false;

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

	return true;
}
