#pragma once

#include <UDP/Reflection/ReflTraits.h>

#include <string>

namespace Ubpa {
	class Scene;
	class SObj;

	class ISerializer : public ReflTraitsVisitor {
	public:
		virtual std::string Serialize(const Scene* scene) = 0;
		virtual std::string Serialize(const SObj* sobj) = 0;
	};
}
