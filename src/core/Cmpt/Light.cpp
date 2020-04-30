#include <UScene/core/Cmpt/Light.h>

#include <UScene/core/SObj.h>

#include "detail/dynamic_reflection/Light.inl"

using namespace Ubpa;
using namespace std;

void Cmpt::Light::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Light();
}

Cmpt::Light::Light() = default;
Cmpt::Light::~Light() { delete light; }

Cmpt::Light::Light(Light && light) noexcept
	: light{ light.light }
{
	light.light= nullptr;
}

void Cmpt::Light::SetLight(Ubpa::Light * light) {
	delete this->light;
	this->light= light;
}
