#pragma once

#include <UGM/transform.h>

namespace Ubpa::UScene {
	struct LocalToParent {
		transformf value{ transformf::eye() };
	};
}

#include "details/LocalToParent_AutoRefl.inl"

