#pragma once

#include "Material.h"

#include <UGM/rgb.h>

namespace Ubpa {
	struct Diffuse : Material {
		rgbf albedo;

		Diffuse(const rgbf& albedo = rgbf{ 1.f })
			: albedo(albedo) {}
	};
}