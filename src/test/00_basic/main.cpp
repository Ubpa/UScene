#include <UScene/core/Scene.h>
#include <UScene/core/Cmpt/Transform.h>

#include <iostream>

using namespace std;
using namespace Ubpa;

class Mover : public Component {
public:
	void OnUpdate(Cmpt::Position* p) {
		p->value += vecf3{ 1,1,1 };
	}
};

int main() {
	Scene scene("scene");

	auto [sobj0, mover] = scene.CreateSObj<Mover>("sobj0");
	auto [sobj1] = scene.CreateSObj<>("sobj1");
	auto [sobj2] = scene.CreateSObj<>("sobj2");

	sobj0->AddChild(sobj1);
	sobj0->AddChild(sobj2);

	cout << sobj0->Get<Cmpt::Position>()->value << endl;
	scene.Update();
	cout << sobj0->Get<Cmpt::Position>()->value << endl;

	return 0;
}
