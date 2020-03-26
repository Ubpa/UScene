#pragma once

#include <UGM/rgba.h>

#include <UDP/Basic/Read.h>

#include <string>

namespace Ubpa {
	class Image {
	public:
		Read<Image, float*> data;
		Read<Image, size_t> width;
		Read<Image, size_t> height;
		Read<Image, size_t> channel;
		std::string path;

		Image();
		virtual ~Image();
		Image(const std::string& path);
		Image(size_t width, size_t height, size_t channel);
		Image(size_t width, size_t height, size_t channel, const float* data);
		Image(Image&& image) noexcept;

		bool Init(const std::string& path);
		void Init(size_t width, size_t height, size_t channel);
		void Init(size_t width, size_t height, size_t channel, const float* data);

		// png, bmp, tga, jpg, hdr
		bool Save();
		bool Save(const std::string& path);
		void Clear();

		bool IsValid() const noexcept;

		float& At(size_t x, size_t y, size_t c);
		float At(size_t x, size_t y, size_t c) const;
		const rgbaf At(size_t x, size_t y) const;

		const rgbaf Sample(float u, float v) const;
	};
}
