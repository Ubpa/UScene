#pragma once

#include <UDP/Visitor.h>

#include <UGM/UGM.h>

namespace Ubpa {
	class BVH;
	class SObj;
	class Primitive;

	class IntersectorClosest {
	public:
		static IntersectorClosest& Instance() {
			static IntersectorClosest instance;
			return instance;
		}

		struct Rst {
			bool IsIntersected() const noexcept { return sobj != nullptr; }
			const SObj* sobj{ nullptr }; // intersection sobj
			const Primitive* primitive{ nullptr }; // triangle
			pointf3 pos; // intersection point's position
			pointf2 uv; // texcoord
			normalf n; // normal, normalized
			vecf3 tangent; // perpendicular to normal, normalized
		};

		const Rst Visit(const BVH* bvh, const rayf3& r) const;

	protected:
		IntersectorClosest();

	private:
		struct Temp {
			Temp(const rayf3& r) :r{ r } {}
			rayf3 r;
			std::array<float, 3> wuv; // triangle
		};

		Visitor<void(Rst&, Temp&)> visitor;
	};
}
