#pragma once

#include "LinearBVHNode.h"
#include "detail/PrimitiveGetter.h"

#include <UGM/bbox.h>
#include <UGM/transform.h>

#include <vector>
#include <unordered_map>

#include <UDP/Visitor/Visitor.h>

namespace Ubpa {
	class SObj;
	class Scene;
	class Primitive;
	class BVHNode;

	class BVH {
	public:
		BVH(Scene* scene = nullptr);
		~BVH();

		void Init(Scene* scene);
		void Clear();

	public:
		const transformf& GetW2L(Primitive* primitive) const;
		const transformf& GetL2W(Primitive* primitive) const;
		const SObj* GetSObj(Primitive* primitive) const;
		const LinearBVHNode& GetNode(size_t idx) const;
		const Primitive* GetPrimitive(size_t idx) const;

	private:
		void LinearizeBVH(BVHNode* bvhNode);

	private:
		std::unordered_map<Primitive*, transformf> p2wl; // world to local matrix
		std::unordered_map<Primitive*, transformf> p2lw; // world to local matrix
		std::unordered_map<Primitive*, SObj*> p2sobj;

		class BVHInitializer;
		friend class BVHInitializer;

		std::vector<Primitive*> primitives;
		std::vector<Primitive*> need_delete_primitives;

		detail::Accel_::PrimitiveGetter primitiveGetter;

		std::vector<LinearBVHNode> linearBVHNodes;
	};
}
