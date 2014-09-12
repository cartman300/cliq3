#pragma once

#ifndef CLIENT
#error Must compile under CLIENT
#endif

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;

namespace CLIq3 {
	public ref class cAddon {
	public:
		virtual void Load() {}
		virtual void Unload() {}

		virtual void Draw3D(IntPtr RefDef) {}
		virtual void Draw2D() {}
	};

	public ref class cAPI {
	private:
		static List<cAddon^>^ Instances = gcnew List<cAddon^>();

	public:
		static void Print(String^ S);
		static void PrintError(String^ S);

	internal:
		static void LoadPlugins();
		static void LoadPlugin(Assembly^ Asm);
		static void UnloadPlugins();

		static void OnDraw3D(IntPtr RefDef);
		static void OnDraw2D();
	};
}