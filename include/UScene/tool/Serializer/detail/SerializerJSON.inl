#pragma once

namespace Ubpa {
	template<typename T>
	void SerializerJSON::ImplVisit(T* const& obj) {
		if (!obj || !ISerializer::IsRegisted(obj))
			writer.Null();
		else
			ISerializer::Visit(obj);
	}

	template<typename Func>
	void SerializerJSON::RegistSerializeOtherMember(Func&& func) {
		using ArgList = FuncTraits_ArgList<Func>;
		static_assert(Length_v<ArgList> == 2,
			"arguments must be (Ubpa::UJsonWriter&, const Obj*)");
		static_assert(std::is_same_v<Front_t<ArgList>, UJsonWriter&>,
			"arguments must be (Ubpa::UJsonWriter&, const Obj*)");
		using CTP = At_t<ArgList, 1>;
		static_assert(std::is_pointer_v<CTP>,
			"arguments must be (Ubpa::UJsonWriter&, const Obj*)");
		using CT = std::remove_pointer_t<CTP>;
		static_assert(std::is_const_v<CT>,
			"arguments must be (Ubpa::UJsonWriter&, const Obj*)");
		using T = std::remove_const_t<CT>;

		callbacks[vtable_of<T>::get()] = [func = std::forward<Func>(func), this](const void* obj) {
			func(writer, reinterpret_cast<CTP>(obj));
		};
	}

	template<typename Obj>
	void SerializerJSON::RegistObjPtrMemVar() {
		VarPtrVisitor<SerializerJSON>::RegistC<Obj*>();
	}
}
