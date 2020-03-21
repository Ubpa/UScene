#include <UScene/core/Cmpt/Camera.h>

using namespace Ubpa;

void Cmpt::Camera::SetFOV(float fov) {
	this->fov = fov;
	Update();
}

void Cmpt::Camera::SetAR(float ar) {
	this->ar = ar;
	Update();
}

void Cmpt::Camera::Init(float fov, float ar, const pointf3& pos, const vecf3& front, const vecf3& worldUp) {
	this->fov = fov;
	this->ar = ar;
	this->pos = pos;

	this->worldUp = worldUp.normalize();
	this->front = front.normalize();
	assert(this->worldUp != this->front);
}

void Cmpt::Camera::Update() {
	auto nRight = front.cross(worldUp);
	auto nUp = nRight.cross(front);

	float height = 2 * std::tanf(fov / 2.f);
	float width = height * ar;

	posToLBCorner = front - (width / 2.f) * nRight - (height / 2.f) * nUp;
	right = width * nRight;
	up = height * nUp;
}
