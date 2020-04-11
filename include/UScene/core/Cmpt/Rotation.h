#pragma once

#include "Component.h"

#include <UGM/quat.h>

namespace Ubpa::Cmpt {
	struct Rotation : Component {
		quatf value{ quatf::identity() };
	};
}
