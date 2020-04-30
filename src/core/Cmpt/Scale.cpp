#include <UScene/core/Cmpt/Scale.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Scale.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Scale::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Scale();
}
