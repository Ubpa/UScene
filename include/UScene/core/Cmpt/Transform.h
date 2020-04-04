#pragma once

#include "../Component.h"

#include <UGM/transform.h>

#include <UDP/Basic/Dirty.h>
#include <UDP/Basic/Read.h>

namespace Ubpa::Cmpt {
	class Transform : public Component {
	public:
		Read<Transform, pointf3> pos{ 0.f };
		Read<Transform, scalef3> scale{ 1.f };
		Read<Transform, quatf> rot{ quatf::identity() };

		Dirty<transformf> tsfm;

		void SetPosition(const pointf3& pos);
		void Move(const vecf3& displacement);
		void SetScale(const scalef3& scale);
		void Scale(const scalef3& scale);
		void SetRotation(const quatf& rot);

		void Init(const pointf3& pos = pointf3{ 0.f },
			const scalef3& scale = scalef3{ 1.f },
			const quatf& rot = quatf::identity());

		const transformf LocalToWorldMatrix() const;
		const pointf3 WorldPos() const;
		const quatf WorldRot() const;
		const vecf3 FrontInWorld() const;
		const vecf3 RightInWorld() const;
		const vecf3 UpInWorld() const;

		Transform();
		Transform(Transform&& tsfm) = default;
	};
}
