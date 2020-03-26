#include <UScene/core/Cmpt/Transform.h>

#include <UScene/core/SObj.h>

using namespace Ubpa;

Cmpt::Transform::Transform()
	: tsfm{[this](transformf& tsfm) { tsfm = transformf(pos, scale, rot); }} { }

void Cmpt::Transform::SetPosition(const pointf3& pos) {
	this->pos = pos;
	tsfm.SetDirty();
}

void Cmpt::Transform::SetScale(const scalef3& scale) {
	this->scale = scale;
	tsfm.SetDirty();
}

void Cmpt::Transform::SetRotation(const quatf& rot) {
	this->rot = rot;
	tsfm.SetDirty();
}

void Cmpt::Transform::Init(const pointf3& pos, const scalef3& scale, const quatf& rot) {
	this->pos = pos;
	this->scale = scale;
	this->rot = rot;
	tsfm.SetDirty();
}

const transformf Cmpt::Transform::GetLocalToWorldMatrix() const {
	auto tsfm = transformf::eye();
	for (auto cur = sobj; cur != nullptr; cur = cur->parent) {
		auto cmpt = cur->Get<Cmpt::Transform>();
		if (cmpt)
			tsfm = cmpt->tsfm.Get() * tsfm;
	}
	return tsfm;
}
