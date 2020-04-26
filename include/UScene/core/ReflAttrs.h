#pragma once
#pragma warning(disable : 5030)

#include <UDP/Reflection/ReflectionBase.h>

namespace Ubpa {
	struct ReflAttr {
		static constexpr char default_value[] = "null"; // ReflectionBase::Meta::default_value
		static constexpr char read_only[] = "read_only"; // ReflectionBase::Meta::read_only
		static constexpr const char not_serialize[] = "not_serialize";
		static constexpr const char not_UI[] = "not_UI";
	};
}
