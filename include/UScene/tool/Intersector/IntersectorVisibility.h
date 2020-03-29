#pragma once

#include <UDP/Visitor/Visitor.h>

#include <UGM/ray.h>

namespace Ubpa {
	class BVH;
	class Primitive;
	class Square;
	class Sphere;
	class Triangle;

	class IntersectorVisibility : public RawPtrVisitor<IntersectorVisibility, Primitive> {
	public:
		IntersectorVisibility();

		bool Visit(const BVH* bvh, const rayf3& r) const;

	protected:
		using RawPtrVisitor<IntersectorVisibility, Primitive>::Visit;
		void ImplVisit(const Square* primitive);
		void ImplVisit(const Sphere* primitive);
		void ImplVisit(const Triangle* primitive);

	private:
		mutable bool isIntersect;
		mutable rayf3 r;
	};
}
