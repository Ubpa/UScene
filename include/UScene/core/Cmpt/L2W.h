#pragma once

#include "Component.h"

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Root;

	// local to world transformation
	class L2W : public Component {
	public:
		[[not_serialize]]
		Read<Root, transformf> value{ transformf::eye() };

		static void OnRegister();

		const pointf3 WorldPos() const noexcept;
		const quatf WorldRot() const noexcept;
		const scalef3 WorldScale() const noexcept;

		// normalized
		const vecf3 FrontInWorld() const noexcept;
		// normalized
		const vecf3 RightInWorld() const noexcept;
		// normalized
		const vecf3 UpInWorld() const noexcept;
	};
}
