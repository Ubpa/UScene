#pragma once

#include "IDeserializer.h"

#include <UDP/Reflection/VarPtrVisitor.h>
#include <UDP/Reflection/Reflection.h>

#include <UGM/UGM>

#include <string>
#include <functional>
#include <map>

namespace Ubpa {
	class UJsonValue;
	class UJsonDoc;

	class DeserializerJSON : public IDeserializer, public VarPtrVisitor<DeserializerJSON> {
	public:
		DeserializerJSON();

		virtual Scene* DeserializeScene(const std::string& json) override;
		virtual SObj* DeserializeSObj(const std::string& json) override;

	protected:
		template<typename T>
		void ImplVisit(T*& obj);

		template<typename T>
		void ImplVisit(T& property);

		template<typename T, size_t N>
		void ImplVisit(val<T, N>& val);

		template<typename T, size_t N>
		void ImplVisit(vec<T, N>& val);

		template<typename T, size_t N>
		void ImplVisit(point<T, N>& val);

		template<typename T, size_t N>
		void ImplVisit(scale<T, N>& val);

		template<typename T>
		void ImplVisit(rgb<T>& val);

		template<typename T>
		void ImplVisit(rgba<T>& val);

		template<typename T>
		void ImplVisit(quat<T>& val);

		template<typename T>
		void ImplVisit(euler<T>& val);

		template<typename T>
		void ImplVisit(normal<T>& val);

		template<typename T, size_t N>
		void ImplVisit(mat<T, N>& val);

		template<typename T>
		void ImplVisit(Ubpa::transform<T>& val);

		void ImplVisit(std::string& val);

	private:
		Scene* ParseScene(const UJsonDoc* doc);
		void ParseSObj(Scene* scene, SObj* sobj, const UJsonValue* value);
		void* ParseObj(const UJsonValue* value);
		void ParseObj(void* obj, const UJsonValue* value);

		// for visitor
		const UJsonValue* cur{ nullptr };
		void* rstObj{ nullptr };

		template<typename Func>
		void RegistParseObj(Func&& func);

		// dynamic
		std::map<std::string, std::function<void*(const UJsonValue* cur)>> type2func;
	};
}
