#pragma once

#include "../Component.h"

#include "../Material/Material.h"

namespace Ubpa::Cmpt {
	class Material : public Component {
	public:
		Ubpa::Material* const volatile material{ nullptr };

		void SetMaterial(Ubpa::Material* material) {
			delete this->material;
			const_cast<Ubpa::Material*&>(this->material) = material;
		}
	};
}
