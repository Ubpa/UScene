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
	Regist<
		bool,
		float, double,
		int8_t, int16_t, int32_t, int64_t,
		uint8_t, uint16_t, uint32_t, uint64_t,

		valf1, valf2, valf3, valf4,
		vali1, vali2, vali3, vali4,
		valu1, valu2, valu3, valu4,

		vecf1, vecf2, vecf3, vecf4,
		veci1, veci2, veci3, veci4,
		vecu1, vecu2, vecu3, vecu4,

		pointu1, pointu2, pointu3, pointu4,
		pointi1, pointi2, pointi3, pointi4,
		pointf1, pointf2, pointf3, pointf4,

		scalef1, scalef2, scalef3, scalef4,

		rgbf, rgbaf,
		quatf, eulerf, normalf,
		matf3, matf4, transformf,

		Primitive*, Light*, Material*,

		Image*, string,

		SObj*>();

	RegistParseObj([](const rapidjson::Value* cur) {
		string path = cur->FindMember("path")->value.GetString();
		return ResourceMngr<Image>::Instance().GetOrCreate(path, path);
	});
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
	for (const auto& cmptObj : value["Components"].GetArray()) {
		auto cmpt = ReflectionMngr::Instance().Create(cmptObj["type"].GetString(), sobj);
		ParseObj(cmpt, cmptObj);
	}

	for (const auto& childObj : value["children"].GetArray()) {
		auto [child] = scene->CreateSObj(childObj["name"].GetString(), sobj);
		ParseSObj(scene, child, childObj);
	}
}

void* DeserializerJSON::ParseObj(const rapidjson::Value& value) {
	if (value.IsNull())
		return nullptr;

	if (!value.HasMember("type")) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "no type" << endl;
		return nullptr;
	}

	const Value& name = value["type"];
	auto target = type2func.find(string{ name.GetString() });
	if (target != type2func.end())
		return target->second(cur);

	void* obj = ReflectionMngr::Instance().Create(name.GetString());
	if (!obj) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "create" << name.GetString() << "fail" << endl;
		return nullptr;
	}

	ParseObj(obj, value);

	return obj;
}

void DeserializerJSON::ParseObj(void* obj, const rapidjson::Value& value) {
	auto refl = ReflectionMngr::Instance().GetReflction(obj);
	if (!refl) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "get " << value["type"].GetString() << "reflection by void* fail" << endl;
		return;
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
}

template<typename T>
void DeserializerJSON::ImplVisit(T*& obj) {
	obj = reinterpret_cast<T*>(ParseObj(*cur));
}

template<typename T>
void DeserializerJSON::DeserializeArray(T& arr, const rapidjson::Value& value) {
	const auto& varr = value.GetArray();
	assert(arr.size() == value.Size());
	for (size_t i = 0; i < arr.size(); i++)
		Set(arr[i], varr[static_cast<SizeType>(i)]);
}

template<typename T, size_t N> void DeserializerJSON::ImplVisit(val<T, N>& val) { DeserializeArray(val, *cur); }
template<typename T, size_t N> void DeserializerJSON::ImplVisit(point<T, N>& val) { DeserializeArray(val, *cur); }
template<typename T, size_t N> void DeserializerJSON::ImplVisit(vec<T, N>& val) { DeserializeArray(val, *cur); }
template<typename T, size_t N> void DeserializerJSON::ImplVisit(scale<T, N>& val) { DeserializeArray(val, *cur); }

template<typename T> void DeserializerJSON::ImplVisit(rgb<T>& val) { DeserializeArray(val, *cur); }
template<typename T> void DeserializerJSON::ImplVisit(rgba<T>& val) { DeserializeArray(val, *cur); }
template<typename T> void DeserializerJSON::ImplVisit(quat<T>& val) { DeserializeArray(val, *cur); }
template<typename T> void DeserializerJSON::ImplVisit(euler<T>& val) { DeserializeArray(val, *cur); }
template<typename T> void DeserializerJSON::ImplVisit(normal<T>& val) { DeserializeArray(val, *cur); }

template<typename T, size_t N> void DeserializerJSON::ImplVisit(mat<T, N>& val) { DeserializeArray(val, *cur); }
template<typename T> void DeserializerJSON::ImplVisit(Ubpa::transform<T>& val) { DeserializeArray(val, *cur); }

void DeserializerJSON::ImplVisit(string& val) { val = cur->GetString(); }

void DeserializerJSON::Set(bool& property, const rapidjson::Value& value) {
	property = value.GetBool();
}

void DeserializerJSON::Set(float& property, const rapidjson::Value& value) {
	property = static_cast<float>(value.GetDouble());
}

void DeserializerJSON::Set(double& property, const rapidjson::Value& value) {
	property = value.GetDouble();
}

void DeserializerJSON::Set(int8_t& property, const rapidjson::Value& value) {
	property = value.GetInt();
}

void DeserializerJSON::Set(int16_t& property, const rapidjson::Value& value) {
	property = value.GetInt();
}

void DeserializerJSON::Set(int32_t& property, const rapidjson::Value& value) {
	property = value.GetInt();
}

void DeserializerJSON::Set(int64_t& property, const rapidjson::Value& value) {
	property = value.GetInt64();
}

void DeserializerJSON::Set(uint8_t& property, const rapidjson::Value& value) {
	property = value.GetUint();
}

void DeserializerJSON::Set(uint16_t& property, const rapidjson::Value& value) {
	property = value.GetUint();
}

void DeserializerJSON::Set(uint32_t& property, const rapidjson::Value& value) {
	property = value.GetUint();
}

void DeserializerJSON::Set(uint64_t& property, const rapidjson::Value& value) {
	property = value.GetUint64();
}

template<typename T>
void DeserializerJSON::Set(T& property, const rapidjson::Value& value) {
	DeserializeArray(property, value);
}
