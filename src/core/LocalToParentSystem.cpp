#include <UScene/Systems/LocalToParentSystem.h>
#include <UScene/Systems/TRSToLocalToWorldSystem.h>

#include <UScene/Systems/TRSToLocalToParentSystem.h>
#include <UScene/Components/LocalToParent.h>
#include <UScene/Components/LocalToWorld.h>
#include <UScene/Components/Parent.h>
#include <UScene/Components/Children.h>

using namespace Ubpa::UScene;

void LocalToParentSystem::ChildLocalToWorld(const transformf& parent_l2w, UECS::Entity e) {
	transformf l2w;
	auto w = GetWorld();
	if (w->entityMngr.Have<LocalToWorld>(e) && w->entityMngr.Have<LocalToParent>(e)) {
		auto child_l2w = w->entityMngr.Get<LocalToWorld>(e);
		auto child_l2p = w->entityMngr.Get<LocalToParent>(e);
		l2w = parent_l2w * child_l2p->value;
		child_l2w->value = l2w;
	}
	else
		l2w = parent_l2w;

	if (w->entityMngr.Have<Children>(e)) {
		auto children = w->entityMngr.Get<Children>(e);
		for (const auto& child : children->value)
			ChildLocalToWorld(l2w, e);
	}
}

void LocalToParentSystem::OnUpdate(UECS::Schedule& schedule) {
	UECS::EntityFilter rootFilter{
		TypeList<>{},      // all
		TypeList<>{},      // any
		TypeList<Parent>{} // none
	};

	schedule
		.InsertNone(TRSToLocalToWorldSystem::SystemFuncName, UECS::CmptType::Of<Parent>)
		.Register([this](const LocalToWorld* l2w, const Children* children) {
			for (const auto& child : children->value)
			ChildLocalToWorld(l2w->value, child);
		}, SystemFuncName, rootFilter)
		.Order(TRSToLocalToParentSystem::SystemFuncName, SystemFuncName);
}
