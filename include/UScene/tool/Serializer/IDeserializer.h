#pragma once

#include <string>

namespace Ubpa {
	class SObj;
	class Scene;

	class IDeserializer {
	public:
		virtual Scene* DeserializeScene(const std::string& path) = 0;
		virtual SObj* DeserializeSObj(const std::string& path) = 0;
	};
}
