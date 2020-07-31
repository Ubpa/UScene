#pragma once

#include <UECS/World.h>

namespace Ubpa {
	namespace UScene {
		class WorldToLocalSystem : public UECS::System {
		public:
			using System::System;

			virtual void OnUpdate(UECS::Schedule& schedule) override;
		};
	}
}
