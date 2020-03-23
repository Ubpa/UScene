#pragma once

namespace Ubpa {
	template<typename T>
	void Serializer_Json::SerializeObj(const T* obj) {
		writer.StartObject();
		if constexpr (std::is_void_v<T>)
			Visit(obj);
		else {
			if (IsRegisted<T>())
				Visit(obj);
		}
		SerializeOtherMember(obj);
		writer.EndObject();
	}

	template<typename T>
	void Serializer_Json::SerializeOtherMember(const T* p) {
		auto target = callbacks.find(TypeID<T>);
		if (target != callbacks.end())
			target->second(reinterpret_cast<const void*>(p));
	}

	template<typename Func>
	void Serializer_Json::RegistSerializeOtherMember(Func&& func) {
		using CTP = Front_t<FuncTraits_ArgList<Func>>;
		static_assert(std::is_pointer_v<CTP>, "arguments must be const T*");
		using CT = std::remove_pointer_t<CTP>;
		static_assert(std::is_const_v<CT>, "arguments must be const T*");
		using T = std::remove_const_t<CT>;
		callbacks[TypeID<T>] = [func = std::forward<Func>(func)](const void* obj) {
			func(reinterpret_cast<CTP>(obj));
		};
	}

	template<typename T>
	void Serializer_Json::SerializeArray(const T& arr) {
		writer.StartArray();
		for (const auto& ele : arr)
			ImplVisit(ele);
		writer.EndArray();
	}
}
