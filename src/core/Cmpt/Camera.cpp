#include <UScene/core/Cmpt/Camera.h>

using namespace Ubpa;

void Cmpt::Camera::Init(float fov, float ar, const pointf3& pos, const vecf3& front, const vecf3& worldUp) {
	const_cast<float&>(this->fov) = fov;
	const_cast<float&>(this->ar) = fov;

	float height = 2 * std::tanf(fov / 2.f);
	float width = height * ar;

	auto nWorldUp = worldUp.normalize();
	auto nFront = front.normalize();
	assert(nFront != nWorldUp);

	auto nRight = nFront.cross(nWorldUp);
	auto nUp = nRight.cross(nFront);

	this->pos = pos;
	posToLBCorner = nFront - (width / 2.f) * nRight - (height / 2.f) * nUp;
	right = width * nRight;
	up = height * nUp;
}
