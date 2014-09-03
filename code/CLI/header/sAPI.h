#pragma once

#include "Enums.h"
#include "Entity.h"

namespace CLIq3 {
	public ref class sAPIAddon {
	public:
		virtual void Load(){}
		virtual void Unload(){}

		virtual void EntityCreated(IntPtr E){}
		virtual bool Command(String^ S){ return false; }
	};

	public ref class sAPI {
	private:
		static List<sAPIAddon^>^ Instances = gcnew List<sAPIAddon^>();

	public:
		ref class ConColor {
		public:
			literal String^ Black = "^0";
			literal String^ Red = "^1";
			literal String^ Green = "^2";
			literal String^ Yellow = "^3";
			literal String^ Blue = "^4";
			literal String^ Cyan = "^5";
			literal String^ Magenta = "^6";
			literal String^ White = "^7";
		};

		static void Print(String^ S);
		static void PrintError(String^ S);

	internal:
		static void LoadPlugins();
		static void LoadPlugin(Assembly^ Asm);
		static void UnloadPlugins();

		static void OnEntityCreated(IntPtr E);
		static bool OnCommand(String^ S);
	};
}