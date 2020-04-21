#pragma once

#include "Component.h"

#include "../Primitive/Primitive.h"

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Geometry : public Component {
	public:
		Read<Geometry, Primitive*> primitive{ nullptr };

		Geometry();
		Geometry(Geometry&& geo) noexcept;
		virtual ~Geometry();

		static void OnRegist();

		void SetPrimitive(Primitive* primitive);
	};
}
