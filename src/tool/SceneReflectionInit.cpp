#include <UScene/tool/SceneReflectionInit.h>

#include <UScene/core.h>

#include <UDP/Reflection/Reflection.h>

using namespace Ubpa;
using namespace std;

void Ubpa::SceneReflectionInit() {
	Scene s("tmp");
	auto [sobj] = s.CreateSObj<>("tmp");

	vtable_of<SObj>::regist(sobj);

	Reflection<SObj>::Instance()
		.Regist(&SObj::name, "name")
		.Regist(&SObj::children, "children");

	Reflection<Scene>::Instance()
		.Regist(&Scene::root, "root")
		.RegistConstructor();

	// =================== Component ===================

	Reflection<Cmpt::SObjPtr>::Instance()
		.Regist(&Cmpt::SObjPtr::sobj, NAMEOF(Cmpt::SObjPtr::sobj).data())
		.Regist(string(NAMEOF(Cmpt::SObjPtr::sobj)) + "::" + Component::Meta::not_serialize, Component::Meta::not_serialize_value)
		.RegistConstructor([](SObj* sobj) {
			return sobj->Get<Cmpt::SObjPtr>();
		});

	Reflection<Cmpt::Position>::Instance()
		.Regist(&Cmpt::Position::value, "value")
		.RegistConstructor([](SObj* sobj) {
			return sobj->Get<Cmpt::Position>();
		});

	Reflection<Cmpt::Rotation>::Instance()
		.Regist(&Cmpt::Rotation::value, "value")
		.RegistConstructor([](SObj* sobj) {
			return sobj->Get<Cmpt::Rotation>();
		});

	Reflection<Cmpt::Scale>::Instance()
		.Regist(&Cmpt::Scale::value, "value")
		.RegistConstructor([](SObj* sobj) {
			return sobj->Get<Cmpt::Scale>();
		});

	Reflection<Cmpt::Transform>::Instance()
		.Regist(&Cmpt::Transform::value, NAMEOF(Cmpt::Transform::value).data())
		.Regist(string(NAMEOF(Cmpt::Transform::value))+"::"+Component::Meta::not_serialize, Component::Meta::not_serialize_value)
		.RegistConstructor([](SObj* sobj) {
			return sobj->Get<Cmpt::Transform>();
		});

	Reflection<Cmpt::L2W>::Instance()
		.Regist(&Cmpt::L2W::value, NAMEOF(Cmpt::L2W::value).data())
		.Regist(string(NAMEOF(Cmpt::L2W::value)) + "::" + Component::Meta::not_serialize, Component::Meta::not_serialize_value)
		.RegistConstructor([](SObj* sobj) {
			return sobj->Get<Cmpt::L2W>();
		});

	Reflection<Cmpt::Camera>::Instance()
		.Regist(&Cmpt::Camera::ar, "ar")
		.Regist(&Cmpt::Camera::fov, "fov")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Camera>();
			return cmpt;
		});

	Reflection<Cmpt::Geometry>::Instance()
		.Regist(&Cmpt::Geometry::primitive, "primitive")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Geometry>();
			return cmpt;
		});

	Reflection<Cmpt::Light>::Instance()
		.Regist(&Cmpt::Light::light, "light")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Light>();
			return cmpt;
		});

	Reflection<Cmpt::Material>::Instance()
		.Regist(&Cmpt::Material::material, "material")
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Material>();
			return cmpt;
		});

	Reflection<Cmpt::Root>::Instance()
		.RegistConstructor([](SObj* sobj) {
			auto [cmpt] = sobj->Attach<Cmpt::Root>();
			return cmpt;
		});

	// =================== Light ===================

	Reflection<Light>::Instance()
		.RegistConstructor();

	Reflection<AreaLight>::Instance()
		.Regist(&AreaLight::intensity, "intensity")
		.Regist(&AreaLight::color, "color")
		.Regist(&AreaLight::texture, "texture")
		.RegistConstructor();

	Reflection<DirLight>::Instance()
		.Regist(&DirLight::intensity, "intensity")
		.Regist(&DirLight::color, "color")
		.RegistConstructor();

	Reflection<EnvLight>::Instance()
		.Regist(&EnvLight::intensity, "intensity")
		.Regist(&EnvLight::color, "color")
		.Regist(&EnvLight::texture, "texture")
		.RegistConstructor();

	Reflection<PointLight>::Instance()
		.Regist(&PointLight::intensity, "intensity")
		.Regist(&PointLight::color, "color")
		.RegistConstructor();

	// =================== Primitive ===================

	Reflection<Primitive>::Instance()
		.RegistConstructor();

	Reflection<Sphere>::Instance()
		.RegistConstructor();

	Reflection<Square>::Instance()
		.RegistConstructor();

	Reflection<Triangle>::Instance()
		.Regist(&Triangle::mesh, "mesh")
		.Regist(&Triangle::indices, "indices")
		.RegistConstructor();

	Reflection<TriMesh>::Instance()
		.Regist(&TriMesh::indices, "indices")
		.Regist(&TriMesh::positions, "positions")
		.Regist(&TriMesh::texcoords, "texcoords")
		.Regist(&TriMesh::normals, "normals")
		.Regist(&TriMesh::tangents, "tangents")
		.RegistConstructor();

	// =================== Material ===================

	Reflection<Material>::Instance()
		.RegistConstructor();

	Reflection<Texture2D>::Instance()
		.Regist(&Texture2D::inv_u, "inv_u")
		.Regist(&Texture2D::inv_v, "inv_v")
		.Regist(&Texture2D::swap_uv, "swap_uv")
		.Regist(&Texture2D::wrap_u, "wrap_u")
		.Regist(&Texture2D::wrap_v, "wrap_v")
		.Regist(&Texture2D::sample_mode, "sample_mode")
		.Regist(&Texture2D::path, "path")
		.Regist(&Texture2D::img, "img")
		.Regist(string("img::") + Component::Meta::not_serialize, Component::Meta::not_serialize_value);
		/*.RegistConstructor([](const string& path) {
			return new Texture2D{ path };
		});*/

	Reflection<stdBRDF>::Instance()
		.Regist(&stdBRDF::albedo_factor, "albedo_factor")
		.Regist(&stdBRDF::albedo_texture, "albedo_texture")
		.Regist(&stdBRDF::roughness_factor, "roughness_factor")
		.Regist(&stdBRDF::roughness_texture, "roughness_texture")
		.Regist(&stdBRDF::metalness_factor, "metalness_factor")
		.Regist(&stdBRDF::metalness_texture, "metalness_texture")
		.Regist(&stdBRDF::normal_map, "normal_map")
		.RegistConstructor();
}
