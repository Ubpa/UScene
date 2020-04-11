#pragma once

#include "Component.h"

#include <UGM/scale.h>

namespace Ubpa::Cmpt {
	struct Scale : Component {
		scalef3 value{ 1.f };
	};
}
