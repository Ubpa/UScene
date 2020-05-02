#pragma once
#pragma warning(disable : 5030)

#include <UDP/Reflection/ReflectionBase.h>

namespace Ubpa {
	struct ReflAttr {
		static constexpr std::string_view default_value = ReflectionBase::Meta::default_value;
		static constexpr std::string_view read_only = ReflectionBase::Meta::read_only;
		static constexpr std::string_view not_serialize = "not_serialize";
		static constexpr std::string_view serialize_kernel = "serialize_kernel"; // must be string
		static constexpr std::string_view not_UI = "not_UI";
		static constexpr std::string_view range = "range";
	};
}
