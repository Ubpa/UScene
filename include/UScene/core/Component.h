#pragma once

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class SObj;

	class Component {
	public:
		Component() = default;
		virtual ~Component() = default;

		Read<SObj, SObj*> sobj;
	};
}
