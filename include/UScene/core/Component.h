#pragma once

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class SObj;

	class Component {
	public:
		Read<SObj, SObj*> sobj{ nullptr };

	protected:
		Component() = default;
		virtual ~Component() = default;
	};
}
