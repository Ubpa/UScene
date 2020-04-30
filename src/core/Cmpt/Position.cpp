#include <UScene/core/Cmpt/Position.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Position.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Position::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Position();
}
