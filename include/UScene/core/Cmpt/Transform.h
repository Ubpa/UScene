#pragma once

#include "Component.h"

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	struct Position;
	struct Rotation;
	struct Scale;

	class Transform : public Component {
	public:
		Read<Transform, transformf> value{ transformf::eye() };

		void OnUpdate(const Position* p, const Rotation* r, const Scale* s);
	};
}
