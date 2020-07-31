#pragma once

#include <UECS/World.h>

namespace Ubpa {
	namespace UScene {
		struct WorldToLocalSystem {
			static void OnUpdate(UECS::Schedule& schedule);
		};
	}
}
