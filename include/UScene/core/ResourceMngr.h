#pragma once

#include <map>
#include <string>

namespace Ubpa {
	template<typename Value, typename Key = std::string>
	class ResourceMngr {
	public:
		static ResourceMngr& Instance() noexcept {
			static ResourceMngr instance;
			return instance;
		}

		~ResourceMngr() {
			for (const auto& [key, val] : key2val)
				delete val;
		}

		template<typename... Args>
		Value* GetOrCreate(const Key& key, Args&&... args) {
			auto target = key2val.find(key);
			if (target != key2val.end())
				return target->second;
			Value* val = new Value(std::forward<Args>(args)...);
			key2val[key] = val;
			return val;
		}

		Value* Get(const Key& key) const {
			auto target = key2val.find(key);
			if (target == key2val.end())
				return nullptr;
			return target->second;
		}

		void Release(const Key& key) {
			auto target = key2val.find(key);
			if (target == key2val.end())
				return;

			delete target->second;
			key2val.erase(target);
		}

		void Clear() {
			for (const auto& [key, val] : key2val)
				delete val;
		}

	private:
		std::map<Key, Value*> key2val;

	private:
		ResourceMngr() = default;
	};
}
