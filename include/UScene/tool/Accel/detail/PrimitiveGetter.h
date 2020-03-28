#pragma once

#include <UDP/Visitor/Visitor.h>
#include <UDP/Basic/Read.h>

namespace Ubpa { class Primitive; }

namespace Ubpa::detail::Accel_ {
	class PrimitiveGetter : public RawPtrVisitor<PrimitiveGetter, Primitive> {
	public:
		PrimitiveGetter();
		const Primitive* Visit(const Primitive* primitive) const;
	private:
		mutable const Primitive* rst;
	protected:
		template<typename T>
		void ImplVisit(const T* primitive) { rst = primitive; }
	};
}