#pragma once
#pragma warning(disable : 5030)

#include <UECS/Entity.h>

#include <UGM/transform.h>

#include <UDP/Basic/Read.h>

#include <string>

namespace Ubpa {
	class Scene;

	class SObj {
	public:
		std::string name;
		[[not_serialize]]
		Read<SObj, SObj*> parent{ nullptr };
		Read<SObj, std::set<SObj*>> children;

		void AddChild(SObj* sobj);
		void ReleaseChild(SObj* sobj);

		bool IsDescendantOf(SObj* sobj) const;

		template<typename Cmpt>
		Cmpt* Get();

		template<typename Cmpt>
		const Cmpt* Get() const;

		const std::vector<std::tuple<void*, size_t>> Components() const;

		// DFS
		template<typename Cmpt>
		SObj* GetSObjInTreeWith();

		template<typename... Cmpts>
		std::tuple<Cmpts *...> Attach();

		template<typename Cmpt>
		Cmpt* GetOrAttach();

		template<typename... Cmpts>
		void Detach();

		bool IsAlive() const noexcept;

		// Attach, Detach, Release, World::CreateEntity
		void AddCommand(const std::function<void()>& command);

		static void OnRegister();
	protected:
		SObj() : entity{ nullptr } {}
		SObj(Scene* scene, Entity* entity, const std::string& name);
		virtual ~SObj();

		Entity* entity;
		Scene* scene{ nullptr };

		friend class Scene;
	};
}

#include "detail/SObj.inl"
