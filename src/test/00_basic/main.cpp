#include <UScene/core/Scene.h>
#include <UScene/core/Cmpt/Transform.h>

#include <iostream>

using namespace std;
using namespace Ubpa;

class Mover : public Component {
public:
	void OnUpdate(Cmpt::Transform* tsfm) {
		cout << "moving" << endl;
		tsfm->Move({ 1,1,1 });
	}
};

int main() {
	Scene scene("scene");

	auto [sobj0, tsfm0, mover] = scene.CreateSObj<Mover>("sobj0");
	auto [sobj1, tsfm1] = scene.CreateSObj<>("sobj1");
	auto [sobj2, tsfm2] = scene.CreateSObj<>("sobj2");

	sobj0->AddChild(sobj1);
	sobj0->AddChild(sobj2);

	tsfm0->Init({ 0.f,0.f,1.f });
	tsfm1->SetScale({ 2.f });
	tsfm2->SetRotation({ vecf3{ 0.f,1.f,0.f }, to_radian(-90.f) });

	tsfm0->LocalToWorldMatrix().print();
	tsfm1->LocalToWorldMatrix().print();
	tsfm2->LocalToWorldMatrix().print();

	cout << tsfm0->pos << endl;
	scene.Update();
	cout << tsfm0->pos << endl;

	return 0;
}
