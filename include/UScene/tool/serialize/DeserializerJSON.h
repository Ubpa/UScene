#pragma once

#include "IDeserializer.h"

#include <UDP/Reflection/VarPtrVisitor.h>
#include <UDP/Reflection/Reflection.h>
#include <rapidjson/document.h>

#include <UGM/UGM>

#include <string>
#include <functional>
#include <map>

namespace Ubpa {
	class DeserializerJSON : public IDeserializer, public VarPtrVisitor<DeserializerJSON> {
	public:
		DeserializerJSON();

		virtual Scene* DeserializeScene(const std::string& json) override;
		virtual SObj* DeserializeSObj(const std::string& json) override;

		template<typename Func>
		void RegistParseObj(Func&& func) {
			using T = std::remove_pointer_t<FuncTraits_Ret<Func>>;
			type2func[Reflection<T>::Instance().GetName()] = [func = std::forward<Func>(func)](const rapidjson::Value* cur)->void* {
				return reinterpret_cast<void*>(func(cur));
			};
		}

	protected:
		template<typename T>
		void DeserializeArray(T& arr, const rapidjson::Value& value);

		template<typename T>
		void ImplVisit(T*& obj);

		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
		void ImplVisit(T& property) { Set(property, *cur); }

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

		void Set(bool& property, const rapidjson::Value& value);
		void Set(float& property, const rapidjson::Value& value);
		void Set(double& property, const rapidjson::Value& value);
		void Set(int8_t& property, const rapidjson::Value& value);
		void Set(int16_t& property, const rapidjson::Value& value);
		void Set(int32_t& property, const rapidjson::Value& value);
		void Set(int64_t& property, const rapidjson::Value& value);
		void Set(uint8_t& property, const rapidjson::Value& value);
		void Set(uint16_t& property, const rapidjson::Value& value);
		void Set(uint32_t& property, const rapidjson::Value& value);
		void Set(uint64_t& property, const rapidjson::Value& value);
		template<typename T> // array
		void Set(T& property, const rapidjson::Value& value);

	private:
		Scene* ParseScene(const rapidjson::Document& doc);
		void ParseSObj(Scene* scene, SObj* sobj, const rapidjson::Value& value);
		void* ParseObj(const rapidjson::Value& value);
		void ParseObj(void* obj, const rapidjson::Value& value);

		// for visitor
		const rapidjson::Value* cur{ nullptr };
		void* rstObj{ nullptr };

		// dynamic
		std::map<std::string, std::function<void* (const rapidjson::Value * cur)>> type2func;
	};
}
