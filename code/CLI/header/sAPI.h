#pragma once

#ifndef SERVER
#error Must compile under SERVER
#endif

#include "Enums.h"
#include "Pointers.h"
#include "Entity.h"
#include "sQuake.h"
#include "ConColor.h"

namespace CLIq3 {
	public ref class sAddon {
	public:
		virtual void Load() {}
		virtual void Unload() {}

		virtual void EntityCreated(EntPtr^ E) {}
		virtual void Command(String^ S) {}
		virtual bool FireWeapon(EntPtr^ Ply, bool AltFire, Vec3 Muzzle, Vec3 Forward) { return false; }
	};

	public ref class sAPI {
	private:
		static List<sAddon^>^ Instances = gcnew List<sAddon^>();

	public:

		static void Print(String^ S);
		static void PrintError(String^ S);

		static int GetTime();
		static Vec3 EvaluateTrajectory(Trajectory T, int Time);

	internal:
		static void LoadPlugins();
		static void LoadPlugin(Assembly^ Asm);
		static void UnloadPlugins();

		static void OnEntityCreated(EntPtr^ Ent);
		static void OnCommand(String^ S);
		static bool OnFireWeapon(EntPtr^ Ent, bool AltFire, Vec3 Muzzle, Vec3 Forward);
	};
}