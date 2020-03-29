#pragma once

namespace Ubpa {
	template<typename Value, typename Key>
	ResourceMngr<Value, Key>& ResourceMngr<Value, Key>::Instance() noexcept {
		static ResourceMngr instance;
		return instance;
	}

	template<typename Value, typename Key>
	ResourceMngr<Value, Key>::~ResourceMngr() {
		for (const auto& [key, val] : key2val)
			delete val;
	}

	template<typename Value, typename Key>
	Value* ResourceMngr<Value, Key>::Get(const Key& key) const {
		auto target = key2val.find(key);
		if (target == key2val.end())
			return nullptr;
		return target->second;
	}

	template<typename Value, typename Key>
	bool ResourceMngr<Value, Key>::Regist(const Key& key, Value* val) {
		if (Get(key))
			return false;
		key2val[key] = val;
		return true;
	}

	template<typename Value, typename Key>
	template<typename... Args>
	Value* ResourceMngr<Value, Key>::GetOrCreate(const Key& key, Args&&... args) {
		Value* rst = Get(key);
		if (!rst) {
			rst = new Value(std::forward<Args>(args)...);
			key2val[key] = rst;
		}
		return rst;
	}

	template<typename Value, typename Key>
	void ResourceMngr<Value, Key>::Release(const Key& key) {
		auto target = key2val.find(key);
		if (target == key2val.end())
			return;

		delete target->second;
		key2val.erase(target);
	}

	template<typename Value, typename Key>
	void ResourceMngr<Value, Key>::Clear() {
		for (const auto& [key, val] : key2val)
			delete val;
	}
}
