#include <UScene/core/Cmpt/Camera.h>

#include <UScene/core/SObj.h>
#include <UScene/core/Cmpt/Transform.h>

using namespace Ubpa;

void Cmpt::Camera::SetFOV(float fov) {
	this->fov = fov;
	Update();
}

void Cmpt::Camera::SetAR(float ar) {
	this->ar = ar;
	Update();
}

void Cmpt::Camera::Init(float fov, float ar) {
	this->fov = fov;
	this->ar = ar;

	auto modelMatrix = sobj->Get<Cmpt::Transform>()->LocalToWorldMatrix();
	pos = modelMatrix.decompose_position();
	auto rotMatrix = modelMatrix.decompose_rotation_matrix();
	front = (rotMatrix * vecf3{ 0.f,0.f,-1.f }).normalize();

	assert(worldUp != front);

	Update();
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
