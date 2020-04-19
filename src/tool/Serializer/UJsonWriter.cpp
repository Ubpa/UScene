#include <UScene/tool/Serializer/UJsonWriter.h>

#include "../_deps/rapidjson/writer.h"
#include "../_deps/rapidjson/stringbuffer.h"

using namespace Ubpa;
using namespace std;

struct UJsonWriter::Impl {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer;
};

UJsonWriter::~UJsonWriter() = default;
UJsonWriter::UJsonWriter(UJsonWriter && ujson) noexcept = default;
UJsonWriter& UJsonWriter::operator=(UJsonWriter && ujson) noexcept = default;

UJsonWriter::UJsonWriter()
	: pImpl(make_unique<UJsonWriter::Impl>()) {}

void UJsonWriter::Clear() {
	PImpl()->buffer.Clear();
	PImpl()->writer.Reset(PImpl()->buffer);
}

string UJsonWriter::Rst() const {
	return PImpl()->buffer.GetString();
}

void UJsonWriter::Key(const char* str) {
	PImpl()->writer.Key(str);
}

void UJsonWriter::Null() {
	PImpl()->writer.Null();
}

void UJsonWriter::Bool(bool v) {
	PImpl()->writer.Bool(v);
}

void UJsonWriter::Double(double v) {
	PImpl()->writer.Double(v);
}

void UJsonWriter::Int(int v) {
	PImpl()->writer.Int(v);
}

void UJsonWriter::Int64(std::int64_t v) {
	PImpl()->writer.Int64(v);
}

void UJsonWriter::Uint(unsigned int v) {
	PImpl()->writer.Uint(v);
}

void UJsonWriter::Uint64(std::uint64_t v) {
	PImpl()->writer.Uint64(v);
}

void UJsonWriter::String(const char* str) {
	PImpl()->writer.String(str);
}

void UJsonWriter::StartObject() {
	PImpl()->writer.StartObject();
}

void UJsonWriter::EndObject() {
	PImpl()->writer.EndObject();
}

void UJsonWriter::StartArray() {
	PImpl()->writer.StartArray();
}

void UJsonWriter::EndArray() {
	PImpl()->writer.EndArray();
}
