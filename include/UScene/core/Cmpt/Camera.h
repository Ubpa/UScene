#pragma once

#include "../Component.h"

#include <UGM/ray.h>

namespace Ubpa::Cmpt {
	class Camera : public Component {
	public:
		const volatile float fov{ Ubpa::to_radian(60.f) }; // field of view in verticle, in radian
		const volatile float ar{ 16.f / 9.f }; // aspect ratio

		void Init(float fov, float ar,
			const pointf3& pos,
			const vecf3& front,
			const vecf3& worldUp = vecf3{ 0,1,0 });

		rayf3 GenRay(float u, float v) const {
			return { pos, posToLBCorner + u * right + v * up };
		}

	private:
		// the imaging plane is 1m away
		pointf3 pos; // camera position
		vecf3 posToLBCorner; // from camera position to left-bottom corner
		vecf3 right; // right.norm() == width
		vecf3 up; // up.norm() == height
	};
}
