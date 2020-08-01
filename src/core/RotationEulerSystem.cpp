#include <UScene/Systems/RotationEulerSystem.h>

#include <UScene/Components/Rotation.h>
#include <UScene/Components/RotationEuler.h>

using namespace Ubpa::UScene;

void RotationEulerSystem::OnUpdate(UECS::Schedule& schedule) {
	schedule.Register([](Rotation* rot, const RotationEuler* rot_euler) {
			rot->value = rot_euler->value.to_quat();
		}, SystemFuncName);
}
