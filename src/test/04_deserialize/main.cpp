#include <UScene/tool/SceneReflectionInit.h>
#include <UScene/tool/serialize/SerializerJSON.h>
#include <UScene/tool/serialize/DeserializerJSON.h>
#include <UScene/core/core>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	SceneReflectionInit();

	Scene scene("scene");

	auto [sobj0, tsfm0, camera] = scene.CreateSObj<Cmpt::Transform, Cmpt::Camera>("sobj0");
	auto [sobj1, tsfm1, geo, mat] = scene.CreateSObj<Cmpt::Transform, Cmpt::Geometry, Cmpt::Material>("sobj1");
	auto [sobj2, tsfm2, light] = scene.CreateSObj<Cmpt::Transform, Cmpt::Light>("sobj2");

	geo->SetPrimitive(new Sphere);
	mat->SetMaterial(new Diffuse{ 1.f });
	light->SetLight(new PointLight{ 1.f, 1.f });

	ISerializer* serializer = new SerializerJSON;
	IDeserializer* deserializer = new DeserializerJSON;

	auto rst = serializer->Serialize(&scene);
	auto rstScene = deserializer->DeserializeScene(rst);
	rst = serializer->Serialize(rstScene);

	ofstream ofs;
	string filename = "../data/test_04_output.uscene";
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
