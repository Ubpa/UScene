#pragma once

#include "../Component.h"

#include <UGM/ray.h>
#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Camera : public Component {
	public:
		Read<Camera, float> fov{ Ubpa::to_radian(60.f) }; // field of view in verticle, in radian
		Read<Camera, float> ar{ 16.f / 9.f }; // aspect ratio

		void SetFOV(float fov);
		void SetAR(float ar);
		void Init(float fov, float ar,
			const pointf3& pos,
			const vecf3& front,
			const vecf3& worldUp = vecf3{ 0,1,0 });

		// call Init() before calling GenRay
		rayf3 GenRay(float u, float v) const {
			return { pos, posToLBCorner + u * right + v * up };
		}

	private:
		// the imaging plane is 1m away
		pointf3 pos; // camera position
		vecf3 front; // normal
		vecf3 worldUp; // world

		// update posToLBCorner, right and up when fov, ar, pos, front and worldUp is set
		void Update();
		vecf3 posToLBCorner; // from camera position to left-bottom corner
		vecf3 right; // right.norm() == width
		vecf3 up; // up.norm() == height
	};
}
