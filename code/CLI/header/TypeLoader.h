#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

namespace CLIq3 {
	public ref class TypeLoader {
	public:

		Assembly^ LoadAssembly(String^ Path, String^ Filter);
	};
}