#pragma once

#include <map>
#include <string>

namespace Ubpa {
	template<typename Value, typename Key = std::string>
	class ResourceMngr {
	public:
		static ResourceMngr& Instance() noexcept;

		~ResourceMngr();

		Value* Get(const Key& key) const;

		bool Regist(const Key& key, Value* val);

		template<typename... Args>
		Value* GetOrCreate(const Key& key, Args&&... args);

		void Release(const Key& key);

		void Clear();

	private:
		std::map<Key, Value*> key2val;

	private:
		ResourceMngr() = default;
	};
}

#include "detail/ResourceMngr.inl"
