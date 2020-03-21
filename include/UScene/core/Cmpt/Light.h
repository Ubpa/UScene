#pragma once

#include "../Component.h"

#include "../Light/Light.h"

namespace Ubpa::Cmpt {

	class Light : public Component {
	public:
		Ubpa::Light * const volatile light{ nullptr };

		void SetLight(Ubpa::Light* light) {
			delete this->light;
			const_cast<Ubpa::Light*&>(this->light) = light;
		}
	};
}
