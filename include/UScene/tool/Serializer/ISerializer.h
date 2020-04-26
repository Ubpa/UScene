#pragma once

#include <string>

namespace Ubpa {
	class Scene;
	class SObj;

	class ISerializer {
	public:
		virtual std::string Serialize(const Scene* scene) = 0;
		virtual std::string Serialize(const SObj* sobj) = 0;
	};
}
