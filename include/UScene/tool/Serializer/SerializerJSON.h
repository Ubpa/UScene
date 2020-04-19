#include <UScene/tool/Serializer/ISerializer.h>

#include "UJsonWriter.h"

#include <UGM/UGM>

#include <UDP/Reflection/Reflection.h>
#include <UDP/Reflection/VarPtrVisitor.h>

#include <set>

namespace Ubpa {
	class Scene;
	class SObj;

	class SerializerJSON : public ISerializer, public VarPtrVisitor<SerializerJSON> {
	public:
		SerializerJSON();

		virtual std::string Serialize(const Scene* scene) override;
		virtual std::string Serialize(const SObj* sobj) override;
		
		// argument must be (Ubpa::UJsonWriter&, const Obj*)
		template<typename Func>
		void RegistSerializeOtherMember(Func&& func);

		template<typename Obj>
		void RegistObjPtrMemVar();

	protected:
		void SerializeOtherMember(const void* obj);
		
		template<typename T>
		void ImplVisit(T* const& obj);

		template<typename T>
		void ImplVisit(const std::set<T>& p);
		template<typename T>
		void ImplVisit(const std::vector<T>& p);
		void ImplVisit(const std::string& p);

		template<typename T>
		void SerializeArray(const T& arr);

		void ImplVisit(const bool& val);

		void ImplVisit(const float& val);
		void ImplVisit(const double& val);

		void ImplVisit(const int8_t& val);
		void ImplVisit(const int16_t& val);
		void ImplVisit(const int32_t& val);
		void ImplVisit(const int64_t& val);

		void ImplVisit(const uint8_t& val);
		void ImplVisit(const uint16_t& val);
		void ImplVisit(const uint32_t& val);
		void ImplVisit(const uint64_t& val);

		template<typename T, size_t N>
		void ImplVisit(const val<T, N>& val);

		template<typename T, size_t N>
		void ImplVisit(const vec<T, N>& val);

		template<typename T, size_t N>
		void ImplVisit(const point<T, N>& val);

		template<typename T, size_t N>
		void ImplVisit(const scale<T, N>& val);

		template<typename T>
		void ImplVisit(const rgb<T>& val);

		template<typename T>
		void ImplVisit(const rgba<T>& val);
		
		template<typename T>
		void ImplVisit(const quat<T>& val);

		template<typename T>
		void ImplVisit(const euler<T>& val);

		template<typename T>
		void ImplVisit(const normal<T>& val);

		template<typename T, size_t N>
		void ImplVisit(const mat<T, N>& val);

		template<typename T>
		void ImplVisit(const Ubpa::transform<T>& val);

	private:
		virtual void Receive(const void* obj, std::string_view name, const xMap<std::string, std::shared_ptr<const VarPtrBase>>& nv) override;

	private:
		UJsonWriter writer;
		std::map<const void*, std::function<void(const void*)>> callbacks; // key is vtable

		using ReflTraitsVisitor::Visit;
		using VarPtrVisitor<SerializerJSON>::RegistC;
	};
}

#include "detail/SerializerJSON.inl"
