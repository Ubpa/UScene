#pragma once

#include "Component.h"

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Position;
	class Rotation;
	class Scale;

	class Transform : public Component {
	public:
		[[not_serialize]]
		Read<Transform, transformf> value{ transformf::eye() };

		static void OnRegister();

		void OnUpdate(const Position* p, const Rotation* r, const Scale* s);
	};
}
