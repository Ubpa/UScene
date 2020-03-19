#include <UScene/core/SObj.h>

#include <UScene/core/Transform.h>

using namespace Ubpa;

const transformf Cmpt::SObj::GetLocalToWorldMatrix() const {
	auto tsfm = transformf::eye();
	for (auto cur = this; cur != nullptr; cur = cur->Parent()) {
		auto cmpt = cur->GetEntity()->Get<Cmpt::Transform>();
		if (cmpt)
			tsfm = cmpt->tsfm.Get() * tsfm;
	}
	return tsfm;
}
