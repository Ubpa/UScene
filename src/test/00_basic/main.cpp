#include <UScene/core/SObj.h>
#include <UScene/core/Transform.h>
#include <UECS/core/World.h>

#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	World w;

	auto [e0, sobj0, tsfm0] = w.CreateEntity<Cmpt::SObj, Cmpt::Transform>();
	auto [e1, sobj1, tsfm1] = w.CreateEntity<Cmpt::SObj, Cmpt::Transform>();
	auto [e2, sobj2, tsfm2] = w.CreateEntity<Cmpt::SObj, Cmpt::Transform>();

	sobj0->AddChild(sobj1);
	sobj0->AddChild(sobj2);

	tsfm0->Init({ 0.f,0.f,1.f });
	tsfm1->SetScale({ 2.f });
	tsfm2->SetRotation({ vecf3{ 0.f,1.f,0.f }, to_radian(-90.f) });

	sobj0->GetLocalToWorldMatrix().print();
	sobj1->GetLocalToWorldMatrix().print();
	sobj2->GetLocalToWorldMatrix().print();

	return 0;
}
