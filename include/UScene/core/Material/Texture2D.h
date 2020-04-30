#pragma once
#include "../ReflAttrs.h"

#include <string>

#include <UGM/point.h>
#include <UBL/Image.h>

namespace Ubpa {
	class Texture2D {
	public:
		enum class WrapMode { Clamp, Repeat, Mirror };
		enum class SampleMode { Nearest, Linear };

		Texture2D() = default;
		Texture2D(const std::string& path);
		virtual ~Texture2D() = default; // virtual -> vtable -> typeid

		bool inv_u{ false };
		bool inv_v{ false };
		bool swap_uv{ false };
		WrapMode wrap_u{ WrapMode::Clamp };
		WrapMode wrap_v{ WrapMode::Clamp };
		SampleMode sample_mode{ SampleMode::Linear };
		Read<Texture2D, std::string> path;
		[[not_serialize]]
		Read<Texture2D, const Image*> img{ nullptr };

		void SetPath(const std::string& path);
		rgbaf Sample(pointf2 uv) const;
		static void OnRegister();
	};
}
