#pragma once

#include "../Component.h"

#include "../Primitive/Primitive.h"

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
