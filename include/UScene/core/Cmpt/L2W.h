#pragma once

#include "Component.h"

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Transform;
	struct SObjPtr;

	class L2W : public Component {
	public:
		Read<L2W, transformf> value;

		void OnUpdate(const Transform* tsfm, const SObjPtr* ptr);

		const pointf3 WorldPos() const;
		const quatf WorldRot() const;
		const vecf3 FrontInWorld() const;
		const vecf3 RightInWorld() const;
		const vecf3 UpInWorld() const;
	};
}
