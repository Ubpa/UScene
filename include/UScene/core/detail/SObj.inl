#pragma once

#include "../Component.h"

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

	template<typename Cmpt>
	Cmpt* SObj::GetOrAttach() {
		auto cmpt = Get<Cmpt>();
		if (!cmpt) {
			cmpt = Attach<Cmpt>();
			cmpt->sobj = this;
		}
		return cmpt;
	}

	template<typename... Cmpts>
	std::tuple<Cmpts *...> SObj::Attach() {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		auto cmpts = entity->Attach<Cmpts...>();
		for (auto cmpt : cmpts)
			cmpt->sobj = this;
		return cmpts;
	}

	template<typename... Cmpts>
	void SObj::Detach() {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		entity->Detach<Cmpts...>();
	}
}