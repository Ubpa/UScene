#include <UScene/core/core>
#include <UScene/tool/Intersector/IntersectorVisibility.h>
#include <UScene/tool/Accel/BVH.h>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	constexpr size_t width = 400;
	constexpr size_t height = 200;

	Scene scene("scene");

	auto [sobj0, tsfm0, camera] = scene.CreateSObj<Cmpt::Camera>("sobj0");
	auto [sobj1, tsfm1, geo1] = scene.CreateSObj<Cmpt::Geometry>("sobj1");
	auto [sobj2, tsfm2, geo2] = scene.CreateSObj<Cmpt::Geometry>("sobj2");
	auto [sobj3, tsfm3, geo3] = scene.CreateSObj<Cmpt::Geometry>("sobj3");

	geo1->SetPrimitive(new Sphere);
	geo2->SetPrimitive(new Square);
	geo3->SetPrimitive(new TriMesh(TriMesh::Type::Cube));
	tsfm0->SetPosition({ 0,0,8 });
	tsfm1->SetPosition({ -4,0,0 });
	tsfm2->SetRotation({ vecf3{1,0,0}, to_radian(45.f) });
	tsfm3->SetPosition({ 4,0,0 });
	tsfm3->SetScale({ 1,2,1 });
	tsfm3->SetRotation({ vecf3{1,2,1}.normalize(), to_radian(45.f) });
	camera->Init(to_radian(60.f), width / static_cast<float>(height));
	
	BVH bvh(&scene);
	IntersectorVisibility intersector;

	Image img(width, height, 1);
	for (size_t j = 0; j < height; j++) {
		float v = j / static_cast<float>(height);
		for (size_t i = 0; i < width; i++) {
			float u = i / static_cast<float>(width);
			rayf3 r = camera->GenRay(u, v);
			bool visibility = intersector.Visit(&bvh, r);
			img.At(i, j, 0) = static_cast<float>(visibility);
		}
	}
	
	img.Save("../data/test_05_visibility_out.png");

	return 0;
}
