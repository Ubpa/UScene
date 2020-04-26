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

void Scene::OnRegist() {
	// Cmpt
	CmptRegister::Instance().Regist<
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
	detail::dynamic_reflection::ReflRegist_Scene();
	SObj::OnRegist();

	// Light
	AreaLight::OnRegist();
	DirLight::OnRegist();
	EnvLight::OnRegist();
	PointLight::OnRegist();

	// Material
	stdBRDF::OnRegist();
	Texture2D::OnRegist();

	// Primitive
	Sphere::OnRegist();
	Square::OnRegist();
	Triangle::OnRegist();
	TriMesh::OnRegist();
}
