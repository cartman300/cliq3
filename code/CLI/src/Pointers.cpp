#include "../../qcommon/q_shared.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

CLIq3::Pointer::Pointer() {
}

CLIq3::Pointer::Pointer(void* Ptr) {
	this->Ptr = Ptr;
}

CLIq3::Pointer^ CLIq3::Pointer::FromIntPtr(IntPtr Ptr) {
	return gcnew Pointer(Ptr.ToPointer());
}

CLIq3::EntPtr^ CLIq3::EntPtr::FromIntPtr(IntPtr Ptr) {
	return gcnew EntPtr(Ptr.ToPointer());
}

CLIq3::ClientPtr^ CLIq3::ClientPtr::FromIntPtr(IntPtr Ptr) {
	return gcnew ClientPtr(Ptr.ToPointer());
}

EntPtr::EntPtr(void* Ent) {
	Ptr = Ent;
}

ClientPtr::ClientPtr(void* Client) {
	Ptr = Client;
}