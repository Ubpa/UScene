#pragma once

#include "Component.h"

#include <UGM/ray.h>
#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	// front is -z in local coordinate
	class Camera : public Component {
	public:
		float fov{ Ubpa::to_radian(60.f) }; // field of view in verticle, in radian
		float ar{ 16.f / 9.f }; // aspect ratio

		struct CoordinateSystem {
			pointf3 pos;
			vecf3 posToLBCorner; // from camera position to left-bottom corner
			vecf3 right; // right.norm() == width
			vecf3 up; // up.norm() == height
		};

		const CoordinateSystem GenCoordinateSystem(const transformf& l2w) const noexcept;

		rayf3 GenRay(float u, float v, const CoordinateSystem& s) const noexcept {
			return { s.pos, (s.posToLBCorner + u * s.right + v * s.up).normalize() };
		}
	};
}
