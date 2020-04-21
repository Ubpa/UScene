#pragma once

#include "Component.h"

#include <UGM/point.h>

namespace Ubpa::Cmpt {
	class Position : Component {
	public:
		pointf3 value{ 0.f };

		static void OnRegist();
	};
}
