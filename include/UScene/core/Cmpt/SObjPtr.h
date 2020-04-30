#pragma once

#include "Component.h"

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class Scene;
	class SObj;
}

namespace Ubpa::Cmpt {
	class SObjPtr : Component {
	public:
		[[not_serialize]]
		Read<Scene, SObj*> value{ nullptr };

		static void OnRegister();
	};
}
