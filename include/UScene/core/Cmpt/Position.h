#pragma once

#include "Component.h"

#include <UGM/point.h>

namespace Ubpa::Cmpt {
	struct Position : Component {
		pointf3 value{ 0.f };
	};
}
