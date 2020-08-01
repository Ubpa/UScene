#pragma once

#include <UGM/quat.h>

namespace Ubpa::UScene {
	struct Rotation {
		quatf value{ quatf::identity() };
	};
}

#include "details/Rotation_AutoRefl.inl"
