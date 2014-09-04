#pragma once

#include "Enums.h"
#include "Pointers.h"
#include "Entity.h"
#include "sQuake.h"

namespace CLIq3 {
	public ref class sAPIAddon {
	public:
		virtual void Load(){}
		virtual void Unload(){}

		virtual void EntityCreated(EntPtr^ E) {}
		virtual bool Command(String^ S) { return false; }
		virtual bool FireWeapon(EntPtr^ Ply, Vec3 Muzzle, Vec3 Forward) { return false; }
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

		static int GetTime();
		static Vec3 EvaluateTrajectory(Trajectory T, int Time);

	internal:
		static void LoadPlugins();
		static void LoadPlugin(Assembly^ Asm);
		static void UnloadPlugins();

		static void OnEntityCreated(EntPtr^ Ent);
		static bool OnCommand(String^ S);
		static bool OnFireWeapon(EntPtr^ Ent, Vec3 Muzzle, Vec3 Forward);
	};
}