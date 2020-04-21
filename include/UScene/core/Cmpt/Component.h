#pragma once
#include "../ReflAttrs.h"

namespace Ubpa {
	// vtable is typeid
	// alignas for __m128
	class alignas(16) Component {
	protected:
		virtual ~Component() = default;
	};
}
