#pragma once

#include "Component.h"

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Transform;
	class L2W;
	class SObjPtr;

	class Root : public Component {
	public:
		static void OnRegist();

		void OnUpdate(const Transform* tsfm, L2W* l2w, const SObjPtr* ptr) const;
	};
}
