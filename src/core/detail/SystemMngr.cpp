#include <UScene/core/detail/SystemMngr.h>

using namespace Ubpa;

void SystemMngr::Update(World* world) {
	for (const auto& [ID, onUpdate] : OnUpdates)
		onUpdate(world);
}
