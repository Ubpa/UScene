#include <UScene/core/Cmpt/SObjPtr.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/SObjPtr.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::SObjPtr::OnRegist() {
	detail::dynamic_reflection::ReflRegist_SObjPtr();
}
