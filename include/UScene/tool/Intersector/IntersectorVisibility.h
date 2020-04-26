#pragma once

#include <UDP/Visitor.h>

#include <UGM/ray.h>

namespace Ubpa {
	class BVH;

	class IntersectorVisibility {
	public:
		static IntersectorVisibility& Instance() noexcept {
			static IntersectorVisibility instance;
			return instance;
		}

		bool Visit(const BVH* bvh, rayf3 r) const;

	private:
		IntersectorVisibility();
		Visitor<bool(const rayf3& r)> visitor;
	};
}
