#pragma once

#include <UGM/quat.h>

namespace Ubpa {
	namespace UScene {
		struct Rotation {
			quatf value{ quatf::identity() };
		};
	}
}

#include "details/Rotation_AutoRefl.inl"
