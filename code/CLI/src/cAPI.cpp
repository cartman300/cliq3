#include "../../qcommon/q_shared.h"
#include "../../cgame/cg_local.h"

using namespace CLIq3;
using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

#define ProtectedCatch catch(Exception^ E) { CLIq3::cAPI::PrintError(String::Format("{0}\n", E->ToString())); }

void cAPI::LoadPlugins() {
	try {
		auto Files = Directory::GetFiles(Path::Combine(Application::StartupPath, BASEGAME), "*.cliq3.dll",
			System::IO::SearchOption::AllDirectories);

		for each (auto F in Files)
			LoadPlugin(Assembly::LoadFrom(F));
	} ProtectedCatch
}

void cAPI::UnloadPlugins() {
	try {
		for each (cAddon^ Pl in Instances) {
			Print(String::Format(S_COLOR_BLUE "Unloading {0}\n", Pl->ToString()));
			Pl->Unload();
		}

		Instances->Clear();
	} ProtectedCatch
}

void cAPI::LoadPlugin(Assembly^ Asm) {
	try {
		auto Typs = Asm->GetExportedTypes();

		for each (auto Typ in Typs) {
			if (Typ->BaseType == cAddon::typeid) {
				Print(String::Format(S_COLOR_BLUE "Loading {0}\n", Typ->ToString()));
				auto Inst = dynamic_cast<cAddon^>(Activator::CreateInstance(Typ));

				if (Inst != nullptr) {
					Inst->Load();
					Instances->Add(Inst);
				} else
					cAPI::Print(String::Format(Str(S_COLOR_BLUE "Could not load {0}"), Typ->ToString()));
			}
		}
	} ProtectedCatch
}

void cAPI::Print(String^ S) {
	auto Ns = (const char*)CreateNString(S);
	Com_Printf("%s", Ns);
	DestroyNString(Ns);
}

void cAPI::PrintError(String^ S) {
	auto Ns = (const char*)CreateNString(S);
	Com_Printf(S_COLOR_RED "CLIENT: %s", Ns);
	DestroyNString(Ns);
}

void cAPI::OnDraw3D(IntPtr RefDef) {
	try {
		for each (cAddon^ A in Instances)
			A->Draw3D(RefDef);
	} ProtectedCatch
}

void cAPI::OnDraw2D() {
	try {
		for each (cAddon^ A in Instances)
			A->Draw2D();
	} ProtectedCatch
}