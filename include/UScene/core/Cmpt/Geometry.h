#pragma once

#include "../Component.h"

#include "../Primitive/Primitive.h"

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Geometry : public Component {
	public:
		Read<Geometry, Primitive*> primitive{ nullptr };

		void SetPrimitive(Primitive* primitive) {
			delete this->primitive;
			this->primitive = primitive;
		}
	};
}
