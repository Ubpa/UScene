#include <UScene/core/Cmpt/Root.h>

#include <UScene/core/Cmpt/SObjPtr.h>
#include <UScene/core/Cmpt/Transform.h>
#include <UScene/core/Cmpt/L2W.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Root.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Root::OnRegist() {
	detail::dynamic_reflection::ReflRegist_Root();
}

void Cmpt::Root::OnUpdate(const Transform* tsfm, L2W* l2w, const SObjPtr* ptr) const {
	stack<SObj*> sobjStack;
	l2w->value= tsfm->value;
	for (auto child : ptr->value->children)
		sobjStack.push(child);
	while (!sobjStack.empty()) {
		auto sobj = sobjStack.top();
		sobjStack.pop();
		sobj->Get<L2W>()->value= sobj->parent->Get<L2W>()->value * sobj->Get<Transform>()->value;
		for (auto child : sobj->children)
			sobjStack.push(child);
	}
}
