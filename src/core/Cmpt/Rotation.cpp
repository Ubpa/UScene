#include <UScene/core/Cmpt/Rotation.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Rotation.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Rotation::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Rotation();
}
