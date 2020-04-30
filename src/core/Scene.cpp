#include <UScene/core/Scene.h>

#include "detail/dynamic_reflection/Scene.inl"

#include <UScene/core.h>

using namespace std;
using namespace Ubpa;

Scene::Scene(const std::string& name)
	: root{ new SObj(this, get<Entity*>(World::CreateEntity<Cmpt::SObjPtr,
			Cmpt::Position, Cmpt::Rotation, Cmpt::Scale,
			Cmpt::Transform, Cmpt::L2W, Cmpt::Root>()), name) }
{
	root->Get<Cmpt::SObjPtr>()->value = root;
}

Scene::~Scene() {
	delete root;
}

void Scene::OnRegister() {
	// Cmpt
	CmptRegistrar::Instance().Register<
		Cmpt::Camera,
		Cmpt::Geometry,
		Cmpt::L2W,
		Cmpt::Light,
		Cmpt::Material,
		Cmpt::Position,
		Cmpt::Root,
		Cmpt::Rotation,
		Cmpt::Scale,
		Cmpt::SObjPtr,
		Cmpt::Transform
	>();

	// SObj, Scene
	detail::dynamic_reflection::ReflRegister_Scene();
	SObj::OnRegister();

	// Light
	AreaLight::OnRegister();
	DirLight::OnRegister();
	EnvLight::OnRegister();
	PointLight::OnRegister();

	// Material
	stdBRDF::OnRegister();
	Texture2D::OnRegister();

	// Primitive
	Sphere::OnRegister();
	Square::OnRegister();
	Triangle::OnRegister();
	TriMesh::OnRegister();
}
