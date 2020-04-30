#pragma once

#include "Component.h"

#include "../Material/Material.h"

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Material : public Component {
	public:
		Read<Material, Ubpa::Material*> material{ nullptr };

		Material();
		virtual ~Material();

		Material(Material&& material) noexcept;

		void SetMaterial(Ubpa::Material* material);

		static void OnRegister();
	};
}
