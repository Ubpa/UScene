#pragma once

#include "Component.h"

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class Scene;
	class SObj;
}

namespace Ubpa::Cmpt {
	struct SObjPtr : Component {
		Read<Scene, SObj*> sobj{ nullptr };
	};
}
