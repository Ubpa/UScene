#include <UScene/core/Transform.h>

using namespace Ubpa;

Cmpt::Transform::Transform(Entity* entity)
	: ECmpt{ entity }, tsfm{[this](transformf& tsfm) { tsfm = transformf(pos, scale, rot); }} { }


void Cmpt::Transform::SetPosition(const pointf3& pos) {
	const_cast<pointf3&>(this->pos) = pos;
	tsfm.SetDirty();
}

void Cmpt::Transform::SetScale(const scalef3& scale) {
	const_cast<scalef3&>(this->scale) = scale;
	tsfm.SetDirty();
}

void Cmpt::Transform::SetRotation(const quatf& rot) {
	const_cast<quatf&>(this->rot) = rot;
	tsfm.SetDirty();
}

void Cmpt::Transform::Init(const pointf3& pos, const scalef3& scale, const quatf& rot) {
	const_cast<pointf3&>(this->pos) = pos;
	const_cast<scalef3&>(this->scale) = scale;
	const_cast<quatf&>(this->rot) = rot;
	tsfm.SetDirty();
}
