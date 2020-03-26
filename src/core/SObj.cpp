#include <UScene/core/SObj.h>

#include <UScene/core/Cmpt/Transform.h>

using namespace Ubpa;
using namespace std;

SObj::SObj(Entity* entity, const string& name)
	: entity(entity), name(name) {}

SObj::~SObj() {
	if(entity && entity->IsAlive())
		entity->Release();
	for (const auto& child : children.get())
		delete child;
}

void SObj::AddChild(SObj* sobj) {
	assert(sobj != this);
	if (sobj->parent)
		sobj->parent->children->erase(sobj);

	sobj->parent = this;
	children->insert(sobj);
}

void SObj::ReleaseChild(SObj* sobj) {
	assert(sobj->parent == this);
	children->erase(sobj);
	delete sobj;
}

bool SObj::IsDescendantOf(SObj* impl) const {
	if (impl == this)
		return true;

	if (parent == nullptr)
		return false;

	return parent->IsDescendantOf(impl);
}
