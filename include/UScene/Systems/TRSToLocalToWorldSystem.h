#pragma once

#include <UECS/World.h>

namespace Ubpa {
	namespace UScene {
		struct TRSToLocalToWorldSystem {
			static void OnUpdate(UECS::Schedule& schedule);
		};
	}
}
