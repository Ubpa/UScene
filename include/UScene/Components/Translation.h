#pragma once

#include <UGM/vec.h>

namespace Ubpa {
	namespace UScene {
		struct Translation {
			vecf3 value{ 0.f };
		};
	}
}

#include "details/Translation_AutoRefl.inl"
