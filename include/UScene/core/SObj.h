#pragma once

#include <UECS/cmpt/Node.h>

#include <UGM/transform.h>

#include <string>

namespace Ubpa::Cmpt {
	class SObj : public Node<SObj> {
	public:
		std::string name;

		using Node<SObj>::Node;

		const transformf GetLocalToWorldMatrix() const;
		const pointf3 GetWorldPos() const { return GetLocalToWorldMatrix().decompose_position(); }
	};
}
