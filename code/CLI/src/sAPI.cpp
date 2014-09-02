
#include "../../qcommon/q_shared.h"
#include "../header/sAPI.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

void sAPI::Print(String^ S) {
	CreateNString(NS, S);
	Com_Printf("%s", NS);
	DestroyNString(NS);
}

void sAPI::LoadPlugins() {
	auto Files = Directory::GetFiles(Str(BASEGAME "/managed"), Str("*.cliq3.dll"),
		System::IO::SearchOption::AllDirectories);

	for each (auto F in Files) 
		LoadPlugin(Assembly::LoadFrom(F));
}

void sAPI::UnloadPlugins() {
	for each (sAPIAddon^ Pl in Instances)
		Pl->Unload();

	Instances->Clear();
}

void sAPI::LoadPlugin(Assembly^ Asm) {
	auto Typs = Asm->GetExportedTypes();

	for each (auto Typ in Typs) {
		if (Typ->BaseType == sAPIAddon::typeid) {
			auto Inst = dynamic_cast<sAPIAddon^>(Activator::CreateInstance(Typ));
			if (Inst != nullptr) {
				Inst->Load();
				Instances->Add(Inst);
			} else
				sAPI::Print(String::Format(Str("Could not load type '{0}'"), Typ->ToString()));
		}
	}
}