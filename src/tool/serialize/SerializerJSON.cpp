#include <UScene/tool/serialize/SerializerJSON.h>

using namespace std;
using namespace Ubpa;
using namespace rapidjson;

SerializerJSON::SerializerJSON() {
	ReflTraitsIniter::Instance().InitC(*this);

	VarPtrVisitor<SerializerJSON>::RegistC<
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

		SObj*,

		Primitive*, Light*, Material*,

		Image*,

		string,
		set<SObj*>,
		vector<pointf3>, vector<pointf2>, vector<normalf>, vector<vecf3>, vector<valu3>>();

	RegistSerializeOtherMember([this](const SObj* sobj) {
		GetWriter().Key("Components");
		GetWriter().StartArray();
		for (auto [cmpt, size] : sobj->Components())
			SerializeObj(cmpt);
		GetWriter().EndArray();
		});
}

string SerializerJSON::Serialize(const Scene* scene) {
	buffer.Clear();
	writer.Reset(buffer);

	SerializeObj(scene);

	return buffer.GetString();
}

string SerializerJSON::Serialize(const SObj* sobj) {
	buffer.Clear();
	writer.Reset(buffer);

	SerializeObj(sobj);

	return buffer.GetString();
}

void SerializerJSON::Receive(const string& name, const map<string, shared_ptr<const VarPtrBase>>& nv) {
	writer.Key("type");
	writer.String(name.c_str());

	for (auto [n, v] : nv) {
		writer.Key(n.c_str());
		VarPtrVisitor<SerializerJSON>::Visit(v);
	}
}

template<typename T>
void SerializerJSON::SerializeObj(const T* obj) {
	if (!obj) {
		writer.Null();
		return;
	}

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
void SerializerJSON::SerializeOtherMember(const T* p) {
	auto target = callbacks.find(TypeID<T>);
	if (target != callbacks.end())
		target->second(reinterpret_cast<const void*>(p));
}

template<typename T>
void SerializerJSON::SerializeArray(const T& arr) {
	writer.StartArray();
	for (const auto& ele : arr)
		ImplVisit(ele);
	writer.EndArray();
}

void SerializerJSON::ImplVisit(const bool& val) { writer.Bool(val); }

void SerializerJSON::ImplVisit(const float& val) { writer.Double(val); }
void SerializerJSON::ImplVisit(const double& val) { writer.Double(val); }

void SerializerJSON::ImplVisit(const int8_t& val) { writer.Int(val); }
void SerializerJSON::ImplVisit(const int16_t& val) { writer.Int(val); }
void SerializerJSON::ImplVisit(const int32_t& val) { writer.Int(val); }
void SerializerJSON::ImplVisit(const int64_t& val) { writer.Int64(val); }

void SerializerJSON::ImplVisit(const uint8_t& val) { writer.Uint(val); }
void SerializerJSON::ImplVisit(const uint16_t& val) { writer.Uint(val); }
void SerializerJSON::ImplVisit(const uint32_t& val) { writer.Uint(val); }
void SerializerJSON::ImplVisit(const uint64_t& val) { writer.Uint64(val); }

template<typename T, size_t N> void SerializerJSON::ImplVisit(const val<T, N>& val) { SerializeArray(val); }
template<typename T, size_t N> void SerializerJSON::ImplVisit(const point<T, N>& val) { SerializeArray(val); }
template<typename T, size_t N> void SerializerJSON::ImplVisit(const vec<T, N>& val) { SerializeArray(val); }
template<typename T, size_t N> void SerializerJSON::ImplVisit(const scale<T, N>& val) { SerializeArray(val); }

template<typename T> void SerializerJSON::ImplVisit(const rgb<T>& val) { SerializeArray(val); }
template<typename T> void SerializerJSON::ImplVisit(const rgba<T>& val) { SerializeArray(val); }
template<typename T> void SerializerJSON::ImplVisit(const quat<T>& val) { SerializeArray(val); }
template<typename T> void SerializerJSON::ImplVisit(const euler<T>& val) { SerializeArray(val); }
template<typename T> void SerializerJSON::ImplVisit(const normal<T>& val) { SerializeArray(val); }

template<typename T, size_t N> void SerializerJSON::ImplVisit(const mat<T, N>& val) { SerializeArray(val); }
template<typename T> void SerializerJSON::ImplVisit(const Ubpa::transform<T>& val) { SerializeArray(val); }

template<typename T> void SerializerJSON::ImplVisit(const std::set<T>& val) { SerializeArray(val); }
template<typename T> void SerializerJSON::ImplVisit(const std::vector<T>& val) { SerializeArray(val); }

void SerializerJSON::ImplVisit(const std::string& val) { writer.String(val.c_str()); }
