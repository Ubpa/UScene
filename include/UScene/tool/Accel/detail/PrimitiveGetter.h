#pragma once

#include <UDP/Visitor/Visitor.h>
#include <UDP/Basic/Read.h>

namespace Ubpa { class Primitive; }

namespace Ubpa::detail::Accel_ {
	class PrimitiveGetter : public RawPtrVisitor<PrimitiveGetter, Primitive> {
	public:
		PrimitiveGetter();
		Primitive* Visit(Primitive* primitive) const;
	private:
		mutable Primitive* rst;
	protected:
		template<typename T>
		void ImplVisit(T* primitive) { rst = primitive; }
	};
}