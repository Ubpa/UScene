#include <UScene/core/Cmpt/Rotation.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Rotation.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Rotation::OnRegist() {
	detail::dynamic_reflection::ReflRegist_Rotation();
}
