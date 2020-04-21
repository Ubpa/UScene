#pragma once

#include "Primitive.h"

#include <UGM/point.h>
#include <UGM/normal.h>

namespace Ubpa {
	// center : (0, 0, 0)
	// side length : 2
	// normal: (0, 1, 0)
	class Square : public Primitive {
	public:
		static constexpr pointf3 center{ 0,0,0 };
		static constexpr normalf normal{ 0,1,0 };
		static constexpr float side_length{ 2.f };

		static void OnRegist();
	};
}
