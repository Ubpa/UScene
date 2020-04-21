#pragma once

#include "Primitive.h"
#include <UGM/point.h>

namespace Ubpa {
	// center : (0, 0, 0)
	// radius : 1
	class Sphere : public Primitive {
	public:
		static constexpr pointf3 center{ 0,0,0 };
		static constexpr float radius{ 1.f };

		static void OnRegist();
	};
}
