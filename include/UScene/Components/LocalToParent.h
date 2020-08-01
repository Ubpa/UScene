#pragma once

#include <UGM/transform.h>

namespace Ubpa {
	namespace UScene {
		struct LocalToParent {
			transformf value{ transformf::eye() };
		};
	}
}

#include "details/LocalToParent_AutoRefl.inl"

