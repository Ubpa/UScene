#pragma once

#include "detail/Dirty.h"

#include "../Component.h"

#include <UGM/transform.h>

namespace Ubpa::Cmpt {
	class Transform : public Component {
	public:
		const pointf3 pos{ 0.f };
		const scalef3 scale{ 1.f };
		const quatf rot{ quatf::identity() };

		detail::UScene::Dirty<transformf> tsfm;

		Transform();

		void SetPosition(const pointf3& pos);
		void SetScale(const scalef3& scale);
		void SetRotation(const quatf& rot);

		void Init(const pointf3& pos = pointf3{ 0.f },
			const scalef3& scale = scalef3{ 1.f },
			const quatf& rot = quatf::identity());

		const transformf GetLocalToWorldMatrix() const;
		const pointf3 GetWorldPos() const { return GetLocalToWorldMatrix().decompose_position(); }
	};
}
