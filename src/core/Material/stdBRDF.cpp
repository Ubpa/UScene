#include <UScene/core/Material/stdBRDF.h>

#include "detail/dynamic_reflection/stdBRDF.inl"

using namespace Ubpa;

void stdBRDF::OnRegist() {
	detail::dynamic_reflection::ReflRegist_stdBRDF();
}
