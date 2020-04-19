#pragma once

namespace Ubpa {
	// vtable is typeid
	// alignas for __m128
	struct alignas(16) Component {
		struct Meta {
			// key: field::meta, value

			static constexpr const char not_serialize[] = "not_serialize";
			static constexpr const char not_serialize_value[] = "true";

			static constexpr const char info[] = "info";
		};

	protected:
		virtual ~Component() = default;
	};
}
