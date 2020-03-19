#pragma once

#include <type_traits>
#include <functional>

namespace Ubpa::detail::UScene {
	template<typename T>
	class Dirty {
	public:
		static_assert(std::is_default_constructible_v<T>);

		Dirty(std::function<void(T & t)> update) : update{ update } {}

		void SetDirty() { dirty = true; }

		const T& Get() const {
			if (dirty) {
				dirty = false;
				update(t);
			}
			return t;
		}

		operator T() const {
			return Get();
		}

	private:
		const std::function<void(T & t)> update;
		mutable bool dirty{ true };
		mutable T t;
	};
}
