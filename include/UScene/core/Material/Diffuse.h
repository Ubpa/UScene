#pragma once

#include "Material.h"

namespace Ubpa {
	struct Diffuse : Material {
		rgbf albedo;

		Diffuse(const rgbf& albedo = rgbf{ 1.f })
			: albedo(albedo) {}
	};
}