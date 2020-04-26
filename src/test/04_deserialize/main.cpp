#include <UScene/tool/Serializer/SerializerJSON.h>
#include <UScene/tool/Serializer/DeserializerJSON.h>
#include <UScene/core.h>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	Scene::OnRegist();

	Scene scene("scene");

	auto [sobj0, camera] = scene.CreateSObj<Cmpt::Camera>("sobj0");
	auto [sobj1, geo1, mat] = scene.CreateSObj<Cmpt::Geometry, Cmpt::Material>("sobj1");
	auto [sobj2, geo2, light] = scene.CreateSObj<Cmpt::Geometry, Cmpt::Light>("sobj2");
	auto [sobj3, geo3] = scene.CreateSObj<Cmpt::Geometry>("sobj3");

	geo1->SetPrimitive(new Sphere);
	auto brdf = new stdBRDF;
	mat->SetMaterial(brdf);
	light->SetLight(new PointLight{ 1.f, 1.f });
	auto cubeMesh = new TriMesh(TriMesh::Type::Cube);
	cubeMesh->SetAndSave("../data/cube_fromcode.obj");
	geo2->SetPrimitive(cubeMesh);
	geo3->SetPrimitive(new TriMesh("../data/cube.obj"));
	scene.Start();
	scene.Update();

	string path = "../data/tex_square.png";
	brdf->albedo_texture = new Texture2D{ path };

	ISerializer* serializer = new SerializerJSON;
	IDeserializer* deserializer = new DeserializerJSON;

	auto rst = serializer->Serialize(&scene);
	auto rstScene = deserializer->DeserializeScene(rst);
	rst = serializer->Serialize(rstScene);

	scene.Stop();

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
