#pragma once

#include <memory>
#include <string>

namespace Ubpa {
	class UJsonWriter {
	public:
		UJsonWriter();
		~UJsonWriter();
		UJsonWriter(UJsonWriter&& ujson) noexcept;
		UJsonWriter& operator=(UJsonWriter&& ujson) noexcept;

		void Clear();
		std::string Rst() const;

		void Key(const char* str);

		void Null();
		void Bool(bool v);
		void Double(double v);
		void Int(int v);
		void Int64(std::int64_t v);
		void Uint(unsigned int v);
		void Uint64(std::uint64_t v);
		void String(const char* str);

		void StartObject();
		void EndObject();
		void StartArray();
		void EndArray();

	private:
		struct Impl;
		Impl* PImpl() { return pImpl.get(); }
		const Impl* PImpl() const { return pImpl.get(); }
		std::unique_ptr<Impl> pImpl;
	};
}
