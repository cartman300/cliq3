
#include "../../qcommon/q_shared.h"
#include "../header/sAPI.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

#define Protected(body) try body catch(Exception^ E) { CLIq3::sAPI::PrintError(String::Format("{0}\n", E->ToString())); }

void sAPI::Print(String^ S) {
	CreateNString(NS, S);
	Com_Printf("%s", NS);
	DestroyNString(NS);
}

void sAPI::PrintError(String^ S) {
	CreateNString(NS, S);
	Com_Printf(S_COLOR_RED "%s", NS);
	DestroyNString(NS);
}

void sAPI::LoadPlugins() {
	Protected({
		auto Files = Directory::GetFiles(Path::Combine(Application::StartupPath, BASEGAME), "*.cliq3.dll",
			System::IO::SearchOption::AllDirectories);

		for each (auto F in Files) {
			Print(String::Format(S_COLOR_GREEN "Loading {0}\n", Path::GetFileName(F)));
			LoadPlugin(Assembly::LoadFrom(F));
		}
	})
}

void sAPI::UnloadPlugins() {
	Protected({
		for each (sAPIAddon^ Pl in Instances) {
			Print(String::Format(S_COLOR_GREEN "Unloading {0}\n", Pl->ToString()));
			Pl->Unload();
		}

		Instances->Clear();
	})
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