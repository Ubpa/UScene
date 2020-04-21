#pragma once

#include "Component.h"

#include <UGM/scale.h>

namespace Ubpa::Cmpt {
	class Scale : Component {
	public:
		scalef3 value{ 1.f };

		static void OnRegist();
	};
}
