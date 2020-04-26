#pragma once

#include "LinearBVHNode.h"

#include <UGM/transform.h>

#include <vector>
#include <unordered_map>

namespace Ubpa {
	class SObj;
	class Scene;
	class Primitive;
	class BVHNode;

	class BVH {
	public:
		BVH(Scene* scene = nullptr);

		void Init(Scene* scene);
		void Clear();

	public:
		const transformf& GetW2L(const Primitive* primitive) const;
		const transformf& GetL2W(const Primitive* primitive) const;
		const SObj* GetSObj(const Primitive* primitive) const;
		const LinearBVHNode& GetNode(size_t idx) const;
		const Primitive* GetPrimitive(size_t idx) const;

	private:
		void LinearizeBVH(const BVHNode* bvhNode);

	private:
		std::unordered_map<const Primitive*, transformf> p2wl; // world to local matrix
		std::unordered_map<const Primitive*, transformf> p2lw; // world to local matrix
		std::unordered_map<const Primitive*, SObj*> p2sobj;

		class BVHInitializer;
		friend class BVHInitializer;

		std::vector<const Primitive*> primitives;

		std::vector<LinearBVHNode> linearBVHNodes;
	};
}
