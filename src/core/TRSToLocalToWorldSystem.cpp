#include <UScene/Systems/TRSToLocalToWorldSystem.h>

#include <UScene/Components/Scale.h>
#include <UScene/Components/LocalToWorld.h>

using namespace Ubpa::UScene;

void TRSToLocalToWorldSystem::OnUpdate(UECS::Schedule& schedule) {
	schedule.Register([](LocalToWorld* l2w, const Scale* s) {
		l2w->value = transformf{ scalef3{s->value} };
	}, "TRSToLocalToWorldSystem::S");
}
