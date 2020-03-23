#include <UScene/tool/serialize/DeserializerJSON.h>

#include <UScene/core/core>

#include <UDP/Reflection/ReflectionMngr.h>
#include <UDP/Reflection/Reflection.h>

#include <rapidjson/error/en.h>

#include <iostream>

using namespace Ubpa;
using namespace std;
using namespace rapidjson;

DeserializerJSON::DeserializerJSON() {
	Regist<SObj*>();
}

Scene* DeserializerJSON::DeserializeScene(const std::string& json) {
	rapidjson::Document doc;

	ParseResult rst = doc.Parse(json.c_str());

	if (!rst) {
		cerr << "ERROR::DeserializerJSON::DeserializeScene:" << endl
			<< "\t" << "JSON parse error: "
			<< GetParseError_En(rst.Code()) << " (" << rst.Offset() << ")" << endl;
		return nullptr;
	}

	auto scene = ParseScene(doc);

	return scene;
}

SObj* DeserializerJSON::DeserializeSObj(const std::string& json) {
	return nullptr;
}


Scene* DeserializerJSON::ParseScene(const rapidjson::Document& doc) {
	if (!doc.IsObject() || !doc.HasMember("type") || doc["type"] != Reflection<Scene>::Instance().GetName().c_str()) {
		cerr << "ERROR::DeserializerJSON::DeserializeScene:" << endl
			<< "\t" << "doc isn't a scene" << endl;
		return nullptr;
	}

	auto scene = new Scene;
	ParseSObj(scene, scene->root, doc["root"]);

	return scene;
}

void DeserializerJSON::ParseSObj(Scene* scene, SObj* sobj, const rapidjson::Value& value) {
	if (!value.IsObject() || !value.HasMember("type") || value["type"] != Reflection<SObj>::Instance().GetName().c_str()) {
		cerr << "ERROR::DeserializerJSON::DeserializeScene:" << endl
			<< "\t" << "value isn't a SObj" << endl;
	}

	sobj->name = value["name"].GetString();
	for (const auto& cmptObj : value["Components"].GetArray())
		auto cmpt = ReflectionMngr::Instance().CreatCustom(cmptObj["type"].GetString(), sobj);

	for (const auto& childObj : value["children"].GetArray()) {
		auto [child] = scene->CreateSObj(childObj["name"].GetString(), sobj);
		ParseSObj(scene, child, childObj);
	}
}

void* DeserializerJSON::ParseObj(const rapidjson::Value& value) {
	if (!value.HasMember("type")) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "no type" << endl;
		return nullptr;
	}

	const Value& name = value["type"];
	void* obj = ReflectionMngr::Instance().Creat(name.GetString());
	if (!obj) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "create" << name.GetString() << "fail" << endl;
		return nullptr;
	}

	auto refl = ReflectionMngr::Instance().GetReflction(obj);
	if (!refl) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "get " << name.GetString() << "reflection by void* fail" << endl;
		return nullptr;
	}

	auto n2v = refl->VarPtrs(obj);
	for (const auto& member : value.GetObject()) {
		if (!std::strcmp(member.name.GetString(), "type")) // equal
			continue;

		auto target = n2v.find(member.name.GetString());
		if (target == n2v.end()) {
			cerr << "WARNNING::DeserializerJSON::ParseObj:" << endl
				<< "\t" << "obj hasn't property (" << member.name.GetString() << ")" << endl;
			continue;
		}

		cur = &member.value;
		Visit(target->second);
	}

	return obj;
}

void DeserializerJSON::ImplVisit(SObj*& val) {
	val = reinterpret_cast<SObj*>(ParseObj(*cur));
}
