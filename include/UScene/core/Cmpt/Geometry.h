#pragma once

#include "../Component.h"

namespace Ubpa { class Primitive; }

namespace Ubpa::Cmpt {

	class Geometry : public Component {
	public:
		Primitive* const volatile primitive{ nullptr };

		void SetPrimitive(Primitive* primitive) {
			delete this->primitive;
			const_cast<Primitive*&>(this->primitive) = primitive;
		}
	};
}
