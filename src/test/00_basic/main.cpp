#include <UScene/UScene.h>
#include <UECS/World.h>

#include <iostream>

using namespace Ubpa::UECS;
using namespace Ubpa::UScene;
using namespace std;

struct PrintSystem {
	static void OnUpdate(Schedule& s) {
		s.Register([](const LocalToWorld* l2w) {
			cout << l2w->value << endl;
		}, "print");
	}
};

int main() {
	RTDCmptTraits::Instance().Register<
		LocalToWorld,
		Scale
	>();

	World w;

	w.systemMngr.Register<
		PrintSystem,
		TRSToLocalToWorldSystem
	>();

	auto [e, l2w, s] = w.entityMngr.Create<LocalToWorld, Scale>();
	s->value = 2.f;
	w.Update();
	cout << w.GenUpdateFrameGraph().Dump() << endl;
}
