#pragma once

#include "../Component.h"

namespace Ubpa { class Light; }

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
