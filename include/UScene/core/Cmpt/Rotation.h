#pragma once

#include "Component.h"

#include <UGM/quat.h>

namespace Ubpa::Cmpt {
	class Rotation : Component {
	public:
		quatf value{ quatf::identity() };

		static void OnRegister();
	};
}
