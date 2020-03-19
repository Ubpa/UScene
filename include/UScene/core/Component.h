#pragma once

namespace Ubpa {
	class SObj;

	class Component {
	public:
		Component() = default;
		virtual ~Component() = default;

		SObj* GetSObj() { return sobj; }
		const SObj* GetSObj() const { return sobj; }

	private:
		SObj* sobj{ nullptr };
		friend class SObj;
	};
}
