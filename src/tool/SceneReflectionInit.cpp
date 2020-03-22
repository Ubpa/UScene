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
		.Regist(&Scene::root, "root");

	// =================== Component ===================

	Reflection<Component>::Instance()
		.SetName("Ubpa::Component");

	Reflection<Cmpt::Camera>::Instance()
		.SetName("Ubpa::Cmpt::Camera")
		.Regist(&Cmpt::Camera::ar, "ar")
		.Regist(&Cmpt::Camera::fov, "fov");

	Reflection<Cmpt::Geometry>::Instance()
		.SetName("Ubpa::Cmpt::Geometry")
		.Regist(&Cmpt::Geometry::primitive, "primitive");

	Reflection<Cmpt::Light>::Instance()
		.SetName("Ubpa::Cmpt::Light")
		.Regist(&Cmpt::Light::light, "light");

	Reflection<Cmpt::Material>::Instance()
		.SetName("Ubpa::Cmpt::Material")
		.Regist(&Cmpt::Material::material, "material");

	Reflection<Cmpt::Transform>::Instance()
		.SetName("Ubpa::Cmpt::Transform")
		.Regist(&Cmpt::Transform::pos, "pos")
		.Regist(&Cmpt::Transform::scale, "scale")
		.Regist(&Cmpt::Transform::rot, "rot");

	// =================== Light ===================

	Reflection<Light>::Instance()
		.SetName("Ubpa::Light");

	Reflection<PointLight>::Instance()
		.SetName("Ubpa::PointLight")
		.Regist(&PointLight::intensity, "intensity")
		.Regist(&PointLight::color, "color");

	// =================== Primitive ===================

	Reflection<Primitive>::Instance()
		.SetName("Ubpa::Primitive");

	Reflection<Sphere>::Instance()
		.SetName("Ubpa::Sphere");

	// =================== Material ===================

	Reflection<Material>::Instance()
		.SetName("Ubpa::Material");

	Reflection<Diffuse>::Instance()
		.SetName("Ubpa::Diffuse")
		.Regist(&Diffuse::albedo, "albedo");
}
