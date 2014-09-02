#include "../../qcommon/q_shared.h"
#include "../header/sAPI.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

#define Protected(body) try body catch(Exception^ E) { CLIq3::sAPI::PrintError(String::Format("{0}\n", E->ToString())); }
#define ToGEntity(P) ((gentity_t*)P.ToPointer())

Entity::Entity(IntPtr E) {
	Ent = E;
}

String^ Entity::GetClassname() {
	return gcnew String(ToGEntity(Ent)->classname);
}

void Entity::SetClassname(String^ S) {
	ToGEntity(Ent)->classname = (char*)CreateNString(S);
}

Entity::DieFunc^ Entity::GetDieFunc() {
	return (Entity::DieFunc^)Marshal::GetDelegateForFunctionPointer(IntPtr(ToGEntity(Ent)->die), Entity::DieFunc::typeid);
}

void Entity::SetDieFunc(Entity::DieFunc^ F) {
	ToGEntity(Ent)->die = (entDieFunc)Marshal::GetFunctionPointerForDelegate(F).ToPointer();
}