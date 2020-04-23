#pragma once

#include <UDP/Visitor/Visitor.h>

#include <UGM/UGM.h>

namespace Ubpa {
	class BVH;
	class SObj;
	class Primitive;
	class Square;
	class Sphere;
	class Triangle;

	class IntersectorClosest : public RawPtrVisitor<IntersectorClosest, Primitive> {
	public:
		IntersectorClosest();

		struct Rst {
			bool IsIntersected() const noexcept { return sobj != nullptr; }
			const SObj* sobj{ nullptr }; // intersection sobj
			pointf3 pos; // intersection point's position
			pointf2 uv; // texcoord
			normalf n; // normal, normalized
			vecf3 tangent; // perpendicular to normal, normalized
		};

		const Rst Visit(const BVH* bvh, const rayf3& r) const;

	protected:
		using RawPtrVisitor<IntersectorClosest, Primitive>::Visit;
		void ImplVisit(const Square* primitive);
		void ImplVisit(const Sphere* primitive);
		void ImplVisit(const Triangle* primitive);

	private:
		mutable rayf3 r;
		mutable Rst rst;
		mutable const Primitive* primitive;
		mutable std::array<float, 3> wuv;
	};
}
