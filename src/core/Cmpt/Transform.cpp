#include <UScene/core/Cmpt/Transform.h>

#include <UScene/core/Cmpt/Position.h>
#include <UScene/core/Cmpt/Rotation.h>
#include <UScene/core/Cmpt/Scale.h>

using namespace Ubpa;

void Cmpt::Transform::OnUpdate(const Position* p, const Rotation* r, const Scale* s) {
	value = transformf(p->value, s->value, r->value);
}
