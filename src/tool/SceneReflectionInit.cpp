#include <UScene/tool/SceneReflectionInit.h>

#include <UScene/core/core>

#include <UDP/Reflection/Reflection.h>

using namespace Ubpa;

void Ubpa::SceneReflectionInit() {
	Scene s("tmp");
	auto [sobj] = s.CreateSObj<>("tmp");

	vtable_of<SObj>::regist(sobj);

	Reflection<SObj>::Instance()
		.SetName("Ubpa::SObj")
		.Regist(&SObj::name, "name")
		.Regist(&SObj::children, "children");

	Reflection<Scene>::Instance()
		.SetName("Ubpa::Scene")
		.Regist(&Scene::root, "root")
		.RegistConstructor();

	// =================== Component ===================

	Reflection<Component>::Instance()
		.SetName("Ubpa::Component");

	Reflection<Cmpt::Camera>::Instance()
		.SetName("Ubpa::Cmpt::Camera")
		.Regist(&Cmpt::Camera::ar, "ar")
		.Regist(&Cmpt::Camera::fov, "fov")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Camera>();
			return cmpt;
		});

	Reflection<Cmpt::Geometry>::Instance()
		.SetName("Ubpa::Cmpt::Geometry")
		.Regist(&Cmpt::Geometry::primitive, "primitive")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Geometry>();
			return cmpt;
		});

	Reflection<Cmpt::Light>::Instance()
		.SetName("Ubpa::Cmpt::Light")
		.Regist(&Cmpt::Light::light, "light")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Light>();
			return cmpt;
		});

	Reflection<Cmpt::Material>::Instance()
		.SetName("Ubpa::Cmpt::Material")
		.Regist(&Cmpt::Material::material, "material")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Material>();
			return cmpt;
		});

	Reflection<Cmpt::Transform>::Instance()
		.SetName("Ubpa::Cmpt::Transform")
		.Regist(&Cmpt::Transform::pos, "pos")
		.Regist(&Cmpt::Transform::scale, "scale")
		.Regist(&Cmpt::Transform::rot, "rot")
		.RegistConstructor([](SObj* sobj) {
		auto [cmpt] = sobj->Attach<Cmpt::Transform>();
		return cmpt;
			});

	// =================== Light ===================

	Reflection<Light>::Instance()
		.SetName("Ubpa::Light")
		.RegistConstructor();

	Reflection<AreaLight>::Instance()
		.SetName("Ubpa::AreaLight")
		.Regist(&AreaLight::intensity, "intensity")
		.Regist(&AreaLight::color, "color")
		.Regist(&AreaLight::texture, "texture")
		.RegistConstructor();

	Reflection<DirLight>::Instance()
		.SetName("Ubpa::DirLight")
		.Regist(&DirLight::intensity, "intensity")
		.Regist(&DirLight::color, "color")
		.RegistConstructor();

	Reflection<EnvLight>::Instance()
		.SetName("Ubpa::EnvLight")
		.Regist(&EnvLight::intensity, "intensity")
		.Regist(&EnvLight::color, "color")
		.Regist(&EnvLight::texture, "texture")
		.RegistConstructor();

	Reflection<PointLight>::Instance()
		.SetName("Ubpa::PointLight")
		.Regist(&PointLight::intensity, "intensity")
		.Regist(&PointLight::color, "color")
		.RegistConstructor();

	// =================== Primitive ===================

	Reflection<Primitive>::Instance()
		.SetName("Ubpa::Primitive")
		.RegistConstructor();

	Reflection<Sphere>::Instance()
		.SetName("Ubpa::Sphere")
		.RegistConstructor();

	Reflection<Square>::Instance()
		.SetName("Ubpa::Square")
		.RegistConstructor();

	Reflection<Triangle>::Instance()
		.SetName("Ubpa::Triangle")
		.Regist(&Triangle::mesh, "mesh")
		.Regist(&Triangle::indices, "indices")
		.RegistConstructor();

	Reflection<TriMesh>::Instance()
		.SetName("Ubpa::TriMesh")
		.Regist(&TriMesh::indices, "indices")
		.Regist(&TriMesh::positions, "positions")
		.Regist(&TriMesh::texcoords, "texcoords")
		.Regist(&TriMesh::normals, "normals")
		.Regist(&TriMesh::tangents, "tangents")
		.RegistConstructor();

	// =================== Material ===================

	Reflection<Material>::Instance()
		.SetName("Ubpa::Material");

	Reflection<stdBRDF>::Instance()
		.SetName("Ubpa::stdBRDF")
		.Regist(&stdBRDF::albedo_factor, "albedo_factor")
		.Regist(&stdBRDF::albedo_texture, "albedo_texture")
		.Regist(&stdBRDF::roughness_factor, "roughness_factor")
		.Regist(&stdBRDF::roughness_texture, "roughness_texture")
		.Regist(&stdBRDF::metalness_factor, "metalness_factor")
		.Regist(&stdBRDF::metalness_texture, "metalness_texture")
		.Regist(&stdBRDF::normal_map, "normal_map")
		.RegistConstructor();

	// =================== Resource ===================

	Reflection<Image>::Instance()
		.SetName("Ubpa::Image")
		.Regist(&Image::width, "width")
		.Regist(&Image::height, "height")
		.Regist(&Image::channel, "channel")
		.Regist(&Image::path, "path")
		.RegistConstructor();
}
