#pragma once

#include <UGM/UGM.h>

namespace Ubpa {
	namespace UScene {
		struct LocalToWorld {
			transformf value{ transformf::eye() };
		};
	}
}

#include "details/LocalToWorld_AutoRefl.inl"
