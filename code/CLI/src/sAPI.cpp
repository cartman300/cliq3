#include "../../qcommon/q_shared.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

#define ProtectedCatch catch(Exception^ E) { CLIq3::sAPI::PrintError(String::Format("{0}\n", E->ToString())); }
#define LEVEL

void sAPI::LoadPlugins() {
	try {
		auto Files = Directory::GetFiles(Path::Combine(Application::StartupPath, BASEGAME), "*.cliq3.dll",
			System::IO::SearchOption::AllDirectories);

		for each (auto F in Files)
			LoadPlugin(Assembly::LoadFrom(F));
	} ProtectedCatch
}

void sAPI::UnloadPlugins() {
	try {
		for each (sAPIAddon^ Pl in Instances) {
			Print(String::Format(S_COLOR_GREEN "Unloading {0}\n", Pl->ToString()));
			Pl->Unload();
		}

		Instances->Clear();
	} ProtectedCatch
}

void sAPI::LoadPlugin(Assembly^ Asm) {
	try {
		auto Typs = Asm->GetExportedTypes();

		for each (auto Typ in Typs) {
			if (Typ->BaseType == sAPIAddon::typeid) {
				Print(String::Format(S_COLOR_GREEN "Loading {0}\n", Typ->ToString()));
				auto Inst = dynamic_cast<sAPIAddon^>(Activator::CreateInstance(Typ));

				if (Inst != nullptr) {
					Inst->Load();
					Instances->Add(Inst);
				} else
					sAPI::Print(String::Format(Str("Could not load {0}"), Typ->ToString()));
			}
		}
	} ProtectedCatch
}

void sAPI::OnEntityCreated(EntPtr^ E) {
	try {
		for each (sAPIAddon^ A in Instances)
			A->EntityCreated(E);
	} ProtectedCatch
}

bool sAPI::OnCommand(String^ S) {
	try {
		bool R = false;
		for each (sAPIAddon^ A in Instances)
			R = R || A->Command(S);
		return R;
	} ProtectedCatch
}

bool sAPI::OnFireWeapon(EntPtr^ Ent, Vec3 Muzzle, Vec3 Forward) {
	try {
		bool R = false;
		for each (sAPIAddon^ A in Instances)
			R = R || A->FireWeapon(Ent, Muzzle, Forward);
		return R;
	} ProtectedCatch
}

void sAPI::Print(String^ S) {
	auto Ns = (const char*)CreateNString(S);
	Com_Printf("%s", Ns);
	DestroyNString(Ns);
}

void sAPI::PrintError(String^ S) {
	auto Ns = (const char*)CreateNString(S);
	Com_Printf(S_COLOR_RED "%s", Ns);
	DestroyNString(Ns);
}

int sAPI::GetTime() {
	return level.time;
}

Vec3 sAPI::EvaluateTrajectory(Trajectory T, int Time) {
	vec3_t V;
	trajectory_t Tr;
	T.ToTrajectoryt(&Tr);
	BG_EvaluateTrajectory(&Tr, Time, V);
	return Vec3::FromVec3t(V);
}