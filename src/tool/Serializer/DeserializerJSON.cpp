#include <UScene/tool/Serializer/DeserializerJSON.h>

#include <UScene/core/SObj.h>
#include <UScene/core/Scene.h>
#include <UScene/core/Primitive/Primitive.h>
#include <UScene/core/Light/Light.h>
#include <UScene/core/Material/Material.h>
#include <UScene/core/Material/Texture2D.h>

#include <UDP/Reflection/ReflectionMngr.h>
#include <UDP/Reflection/Reflection.h>

#include "../_deps/rapidjson/document.h"
#include "../_deps/rapidjson/error/en.h"

#include <iostream>

namespace Ubpa::detail::DeserializerJSON_ {
	template<typename T>
	void DeserializeArray(T& arr, const rapidjson::Value& value);
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
	template<typename T>
	void Set(std::vector<T>& property, const rapidjson::Value& value);
}

using namespace Ubpa;
using namespace std;
using namespace rapidjson;
using namespace Ubpa::detail::DeserializerJSON_;


class Ubpa::UJsonValue {
public:
	UJsonValue(const rapidjson::Value* data) :data{ data } {}
	const rapidjson::Value* operator->() const { return data; }
	const rapidjson::Value& operator*() const { return *data; }
	const rapidjson::Value* data;
};

class Ubpa::UJsonDoc {
public:
	UJsonDoc(rapidjson::Document* data) :data{ data } {}
	rapidjson::Document* operator->() const { return data; }
	rapidjson::Document& operator*() const { return *data; }
	rapidjson::Document* data;
};

template<typename Func>
void DeserializerJSON::RegistGenObj(Func&& func) {
	using T = std::remove_pointer_t<FuncTraits_Ret<Func>>;
	type2func[string(Reflection<T>::Instance().Name())] = [func = std::forward<Func>(func)](const UJsonValue* cur)->void* {
		return reinterpret_cast<void*>(func(cur->data));
	};
}

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

		Texture2D*, string,

		SObj*,

		vector<pointf3>, vector<pointf2>, vector<normalf>, vector<vecf3>, vector<valu3>>();

	RegistGenObj([](const rapidjson::Value* obj) {
		string path = obj->FindMember("path")->value.GetString();
		return new Texture2D{ path };
	});
}

Scene* DeserializerJSON::DeserializeScene(const std::string& json) {
	rapidjson::Document doc;
	UJsonDoc udoc{ &doc };

	ParseResult rst = doc.Parse(json.c_str());

	if (!rst) {
		cerr << "ERROR::DeserializerJSON::DeserializeScene:" << endl
			<< "\t" << "JSON parse error: "
			<< GetParseError_En(rst.Code()) << " (" << rst.Offset() << ")" << endl;
		return nullptr;
	}

	auto scene = ParseScene(&udoc);

	return scene;
}

SObj* DeserializerJSON::DeserializeSObj(const std::string& json) {
	return nullptr;
}

Scene* DeserializerJSON::ParseScene(const UJsonDoc* doc) {
	if (!(**doc).IsObject() || !(**doc).HasMember("type") || (**doc)["type"] != Reflection<Scene>::Instance().Name().data()) {
		cerr << "ERROR::DeserializerJSON::DeserializeScene:" << endl
			<< "\t" << "doc isn't a scene" << endl;
		return nullptr;
	}

	auto scene = new Scene;
	auto val = UJsonValue(&(**doc)["root"]);
	ParseSObj(scene, scene->root, &val);

	return scene;
}

void DeserializerJSON::ParseSObj(Scene* scene, SObj* sobj, const UJsonValue* value) {
	if (!(**value).IsObject() || !(**value).HasMember("type") || (**value)["type"] != Reflection<SObj>::Instance().Name().data()) {
		cerr << "ERROR::DeserializerJSON::DeserializeScene:" << endl
			<< "\t" << "value isn't a SObj" << endl;
	}

	sobj->name = (**value)["name"].GetString();
	for (const auto& cmptObj : (**value)["Components"].GetArray()) {
		auto cmpt = ReflectionMngr::Instance().Create(cmptObj["type"].GetString(), sobj);
		auto val = UJsonValue(&cmptObj);
		ParseObj(cmpt, &val);
	}

	for (const auto& childObj : (**value)["children"].GetArray()) {
		auto [child] = scene->CreateSObj(childObj["name"].GetString(), sobj);
		auto val = UJsonValue(&childObj);
		ParseSObj(scene, child, &val);
	}
}

void* DeserializerJSON::ParseObj(const UJsonValue* value) {
	if ((**value).IsNull())
		return nullptr;

	if (!(**value).HasMember("type")) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "no type" << endl;
		return nullptr;
	}

	const Value& name = (**value)["type"];
	void* obj;
	auto target = type2func.find(string{ name.GetString() });
	if (target != type2func.end())
		obj = target->second(cur);
	else
		obj = ReflectionMngr::Instance().Create(name.GetString());

	if (!obj) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "create" << name.GetString() << "fail" << endl;
		return nullptr;
	}

	ParseObj(obj, value);

	return obj;
}

void DeserializerJSON::ParseObj(void* obj, const UJsonValue* value) {
	auto refl = ReflectionMngr::Instance().GetReflction(obj);
	if (!refl) {
		cerr << "ERROR::DeserializerJSON::ParseObj:" << endl
			<< "\t" << "get " << (**value)["type"].GetString() << "reflection by void* fail" << endl;
		return;
	}

	auto n2v = refl->VarPtrs(obj);
	for (const auto& member : (**value).GetObject()) {
		if (!std::strcmp(member.name.GetString(), "type")) // equal
			continue;
		if (ReflectionMngr::Instance().GetReflction(obj)->FieldMeta(string(member.name.GetString()), ReflAttr::not_serialize) == ReflAttr::default_value)
			continue;

		auto target = n2v.find(member.name.GetString());
		if (target == n2v.end()) {
			cerr << "WARNNING::DeserializerJSON::ParseObj:" << endl
				<< "\t" << "obj hasn't property (" << member.name.GetString() << ")" << endl;
			continue;
		}
		UJsonValue val(&member.value);
		cur = &val;
		Visit(target->second);
	}
}

template<typename T>
void DeserializerJSON::ImplVisit(T*& obj) {
	obj = reinterpret_cast<T*>(ParseObj(cur));
}

template<typename T>
void DeserializerJSON::ImplVisit(T& property) {
	Set(property, **cur);
}

template<typename T>
void Ubpa::detail::DeserializerJSON_::DeserializeArray(T& arr, const rapidjson::Value& value) {
	const auto& varr = value.GetArray();
	assert(arr.size() == value.Size());
	for (size_t i = 0; i < arr.size(); i++)
		Set(arr[i], varr[static_cast<SizeType>(i)]);
}

template<typename T, size_t N> void DeserializerJSON::ImplVisit(val<T, N>& val) { DeserializeArray(val, **cur); }
template<typename T, size_t N> void DeserializerJSON::ImplVisit(point<T, N>& val) { DeserializeArray(val, **cur); }
template<typename T, size_t N> void DeserializerJSON::ImplVisit(vec<T, N>& val) { DeserializeArray(val, **cur); }
template<typename T, size_t N> void DeserializerJSON::ImplVisit(scale<T, N>& val) { DeserializeArray(val, **cur); }

template<typename T> void DeserializerJSON::ImplVisit(rgb<T>& val) { DeserializeArray(val, **cur); }
template<typename T> void DeserializerJSON::ImplVisit(rgba<T>& val) { DeserializeArray(val, **cur); }
template<typename T> void DeserializerJSON::ImplVisit(quat<T>& val) { DeserializeArray(val, **cur); }
template<typename T> void DeserializerJSON::ImplVisit(euler<T>& val) { DeserializeArray(val, **cur); }
template<typename T> void DeserializerJSON::ImplVisit(normal<T>& val) { DeserializeArray(val, **cur); }

template<typename T, size_t N> void DeserializerJSON::ImplVisit(mat<T, N>& val) { DeserializeArray(val, **cur); }
template<typename T> void DeserializerJSON::ImplVisit(Ubpa::transform<T>& val) { DeserializeArray(val, **cur); }

void DeserializerJSON::ImplVisit(string& val) { val = (**cur).GetString(); }

void Ubpa::detail::DeserializerJSON_::Set(bool& property, const rapidjson::Value& value) {
	property = value.GetBool();
}

void Ubpa::detail::DeserializerJSON_::Set(float& property, const rapidjson::Value& value) {
	property = static_cast<float>(value.GetDouble());
}

void Ubpa::detail::DeserializerJSON_::Set(double& property, const rapidjson::Value& value) {
	property = value.GetDouble();
}

void Ubpa::detail::DeserializerJSON_::Set(int8_t& property, const rapidjson::Value& value) {
	property = value.GetInt();
}

void Ubpa::detail::DeserializerJSON_::Set(int16_t& property, const rapidjson::Value& value) {
	property = value.GetInt();
}

void Ubpa::detail::DeserializerJSON_::Set(int32_t& property, const rapidjson::Value& value) {
	property = value.GetInt();
}

void Ubpa::detail::DeserializerJSON_::Set(int64_t& property, const rapidjson::Value& value) {
	property = value.GetInt64();
}

void Ubpa::detail::DeserializerJSON_::Set(uint8_t& property, const rapidjson::Value& value) {
	property = value.GetUint();
}

void Ubpa::detail::DeserializerJSON_::Set(uint16_t& property, const rapidjson::Value& value) {
	property = value.GetUint();
}

void Ubpa::detail::DeserializerJSON_::Set(uint32_t& property, const rapidjson::Value& value) {
	property = value.GetUint();
}

void Ubpa::detail::DeserializerJSON_::Set(uint64_t& property, const rapidjson::Value& value) {
	property = value.GetUint64();
}

template<typename T>
void Ubpa::detail::DeserializerJSON_::Set(T& property, const rapidjson::Value& value) {
	DeserializeArray(property, value);
}

template<typename T>
void Ubpa::detail::DeserializerJSON_::Set(std::vector<T>& property, const rapidjson::Value& value) {
	property.resize(value.GetArray().Size());
	DeserializeArray(property, value);
}
