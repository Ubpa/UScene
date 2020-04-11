#include <UScene/core/Scene.h>

using namespace std;
using namespace Ubpa;

Scene::Scene(const std::string& name)
	: root{ new SObj(this, get<Entity*>(World::CreateEntity<Cmpt::SObjPtr,
			Cmpt::Position, Cmpt::Rotation, Cmpt::Scale,
			Cmpt::Transform, Cmpt::L2W>()), name) }
{
	root->Get<Cmpt::SObjPtr>()->sobj.val = root;
}

Scene::~Scene() {
	delete root;
}
