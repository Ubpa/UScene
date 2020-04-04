#include <UScene/core/Scene.h>

using namespace std;
using namespace Ubpa;

Scene::Scene(const std::string& name)
	: root{ new SObj(this, get<0>(World::CreateEntity<>()), name) } {}

Scene::~Scene() {
	delete root;
}

void Scene::Update() {
	SystemMngr::Instance().Update(this);
}
