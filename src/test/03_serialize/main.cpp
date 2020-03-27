#include <UScene/tool/SceneReflectionInit.h>
#include <UScene/tool/serialize/SerializerJSON.h>
#include <UScene/core/core>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	SceneReflectionInit();

	Scene scene("scene");

	auto [sobj0, tsfm0, camera] = scene.CreateSObj<Cmpt::Transform, Cmpt::Camera>("sobj0");
	auto [sobj1, tsfm1, geo0, mat] = scene.CreateSObj<Cmpt::Transform, Cmpt::Geometry, Cmpt::Material>("sobj1");
	auto [sobj2, tsfm2, geo1, light] = scene.CreateSObj<Cmpt::Transform, Cmpt::Geometry, Cmpt::Light>("sobj2");

	geo0->SetPrimitive(new Sphere);
	auto brdf = new stdBRDF;
	mat->SetMaterial(brdf);
	light->SetLight(new PointLight{ 1.f, 1.f });
	geo1->SetPrimitive(new TriMesh(TriMesh::Type::Cube));

	string path = "../data/tex_square.png";
	brdf->albedo_texture = ResourceMngr<Image>::Instance().GetOrCreate(path, path);

	ISerializer* serializer = new SerializerJSON;

	auto rst = serializer->Serialize(&scene);

	ofstream ofs;
	string filename = "../data/test_03_output.uscene";
	ofs.open(filename);
	if (!ofs.is_open()) {
		cerr << "ERROR::main:" << endl
			<< "\t" << "open file (" << filename << ") fail" << endl;
		return 1;
	}
	ofs << rst;
	ofs.close();

	cout << rst << endl;
	cout << "--------------------------" << endl;
	cout << "test online: https://www.json.cn/" << endl;
	cout << "save to " << filename << endl;
	return 0;
}
