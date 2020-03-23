#pragma once

#include "IDeserializer.h"

#include <UDP/Reflection/VarPtrVisitor.h>
#include <rapidjson/document.h>

namespace Ubpa {
	class DeserializerJSON : public IDeserializer, public VarPtrVisitor<DeserializerJSON> {
	public:
		DeserializerJSON();

		virtual Scene* DeserializeScene(const std::string& json) override;
		virtual SObj* DeserializeSObj(const std::string& json) override;

	protected:
		void ImplVisit(SObj*& val);

	private:
		Scene* ParseScene(const rapidjson::Document& doc);
		void ParseSObj(Scene* scene, SObj* sobj, const rapidjson::Value& value);
		void* ParseObj(const rapidjson::Value& value);

		//rapidjson::Document doc;
		const rapidjson::Value* cur{ nullptr };
	};
}
