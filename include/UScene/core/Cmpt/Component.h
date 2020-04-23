#pragma once
#include "../ReflAttrs.h"

#include <UECS/CmptTag.h>

namespace Ubpa {
	// vtable is typeid
	// alignas for __m128
	class alignas(16) Component {
	protected:
		virtual ~Component() = default;
	};
}
