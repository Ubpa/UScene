#pragma once

#include "../Cmpt/Position.h"
#include "../Cmpt/Rotation.h"
#include "../Cmpt/Scale.h"
#include "../Cmpt/Transform.h"
#include "../Cmpt/L2W.h"
#include "../Cmpt/SObjPtr.h"
#include "../Cmpt/Root.h"

namespace Ubpa::detail::SObj_ {
	template<typename T>
	constexpr bool IsNecessaryCmpt = std::is_same_v<Cmpt::Position, T>
		|| std::is_same_v<Cmpt::Rotation, T>
		|| std::is_same_v<Cmpt::Scale, T>
		|| std::is_same_v<Cmpt::SObjPtr, T>
		|| std::is_same_v<Cmpt::Transform, T>
		|| std::is_same_v<Cmpt::L2W, T>;

	template<typename T>
	constexpr bool IsUndetachableCmpt = IsNecessaryCmpt<T>
		|| std::is_same_v<Cmpt::Root, T>;
}

namespace Ubpa {
	template<typename Cmpt>
	Cmpt* SObj::Get() {
		static_assert(std::is_base_of_v<Component, Cmpt>);
		return entity->Get<Cmpt>();
	}

	template<typename Cmpt>
	const Cmpt* SObj::Get() const {
		static_assert(std::is_base_of_v<Component, Cmpt>);
		return const_cast<SObj*>(this)->Get<Cmpt>();
	}

	template<typename... Cmpts>
	std::tuple<Cmpts *...> SObj::Attach() {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		static_assert(((!detail::SObj_::IsNecessaryCmpt<Cmpts>) &&...),
			"<Cmpts> is necessary component");
		auto cmpts = entity->Attach<Cmpts...>();
		return cmpts;
	}

	template<typename Cmpt>
	Cmpt* SObj::GetOrAttach() {
		auto cmpt = Get<Cmpt>();
		if (!cmpt)
			std::tie(cmpt) = Attach<Cmpt>();
		return cmpt;
	}

	template<typename... Cmpts>
	void SObj::Detach() {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		static_assert(((!detail::SObj_::IsUndetachableCmpt<Cmpts>) &&...),
			"<Cmpts> is undetachable component");
		entity->Detach<Cmpts...>();
	}

	template<typename Cmpt>
	SObj* SObj::GetSObjInTreeWith() {
		auto cmpt = Get<Cmpt>();
		if (cmpt)
			return this;
		for (auto child : children) {
			auto sobj = child->GetSObjInTreeWith<Cmpt>();
			if (sobj)
				return sobj;
		}
		return nullptr;
	}
}
