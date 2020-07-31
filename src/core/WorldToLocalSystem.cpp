#include <UScene/Systems/WorldToLocalSystem.h>

#include <UScene/Components/LocalToWorld.h>
#include <UScene/Components/WorldToLocal.h>

using namespace Ubpa::UScene;

void WorldToLocalSystem::OnUpdate(UECS::Schedule& schedule) {
	schedule.Register([](WorldToLocal* w2l, const LocalToWorld* l2w) {
		w2l->value = l2w->value.inverse();
	}, "TRSToWorldToLocalSystem");
}
