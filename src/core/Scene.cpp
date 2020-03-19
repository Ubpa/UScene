#include <UScene/core/Scene.h>

using namespace std;
using namespace Ubpa;

Scene::Scene(const std::string& name)
	: root(get<1>(World::CreateEntity<Cmpt::SObj>()))
{
	root->name = name;
}
