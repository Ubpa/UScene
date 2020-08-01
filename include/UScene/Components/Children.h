#pragma once

#include <UECS/Entity.h>
#include <vector>

namespace Ubpa {
	namespace UScene {
		struct Children {
			std::vector<UECS::Entity> value;
		};
	}
}

#include "details/Children_AutoRefl.inl"
