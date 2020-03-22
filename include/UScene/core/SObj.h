#pragma once

#include <UECS/Entity.h>

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

#include <string>

namespace Ubpa {
	class Scene;

	class SObj {
	public:
		std::string name;
		Read<SObj, SObj*> parent{ nullptr };
		Read<SObj, std::set<SObj*>> children;

		void AddChild(SObj* sobj);
		void ReleaseChild(SObj* sobj);

		bool IsDescendantOf(SObj* sobj) const;

		template<typename Cmpt>
		Cmpt* Get();

		template<typename Cmpt>
		const Cmpt* Get() const;

		const std::vector<std::tuple<void*, size_t>> Components() const { return entity->Components(); }

		template<typename... Cmpts>
		std::tuple<Cmpts *...> Attach();

		template<typename Cmpt>
		Cmpt* GetOrAttach();

		template<typename... Cmpts>
		void Detach();

		bool IsAlive() const noexcept { return entity->IsAlive(); }

	protected:
		SObj(Entity* entity, const std::string& name);
		virtual ~SObj();
		Entity* entity;
		friend class Scene;
	};
}

#include "detail/SObj.inl"
