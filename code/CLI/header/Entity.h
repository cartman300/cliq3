#pragma once

#ifndef SERVER
#error Must compile under SERVER
#endif

#define GEntityType EntPtr^
#define DeclareFunc(T) \
	[System::Runtime::CompilerServices::Extension] \
	static T^ Get##T(EntPtr^ Ent); \
	[System::Runtime::CompilerServices::Extension] \
	static void Set##T(EntPtr^ Ent, T^ Func)
#define METHOD [System::Runtime::CompilerServices::Extension] static
#define DELEGATE public delegate

namespace CLIq3 {
	DELEGATE void ThinkFunc(IntPtr self);
	DELEGATE void ReachedFunc(IntPtr self);
	DELEGATE void BlockedFunc(IntPtr self, IntPtr other);
	DELEGATE void TouchFunc(IntPtr self, IntPtr other, IntPtr trace);
	DELEGATE void UseFunc(IntPtr self, IntPtr other, IntPtr activator);
	DELEGATE void PainFunc(IntPtr self, IntPtr attacker, int damage);
	DELEGATE void DieFunc(IntPtr self, IntPtr inflictor, IntPtr attacker, int damage, int mod);

	[System::Runtime::CompilerServices::Extension]
	public ref class Entity abstract sealed {
	public:
		METHOD GEntityType Spawn();
		METHOD void Free(GEntityType Ent);

		METHOD ClientPtr^ GetClient(GEntityType Ent);
		METHOD void SetClient(GEntityType Ent, ClientPtr^ Client);

		METHOD bool GetInUse(GEntityType Ent);
		METHOD void SetInUse(GEntityType Ent, bool B);

		METHOD String^ GetClassname(GEntityType Ent);
		METHOD void SetClassname(GEntityType Ent, String^ S);

		METHOD int GetNextThink(GEntityType Ent);
		METHOD void SetNextThink(GEntityType Ent, int Time);

		METHOD EntType GetEType(GEntityType Ent);
		METHOD void SetEType(GEntityType Ent, EntType T);

		METHOD SVFlags GetSVFlags(GEntityType Ent);
		METHOD void SetSVFlags(GEntityType Ent, SVFlags Flags);

		METHOD Weapon GetWeapon(GEntityType Ent);
		METHOD void SetWeapon(GEntityType Ent, Weapon W);

		METHOD int GetOwnerNum(GEntityType Ent);
		METHOD void SetOwnerNum(GEntityType Ent, int O);

		METHOD GEntityType GetParent(GEntityType Ent);
		METHOD void SetParent(GEntityType Ent, GEntityType Parent);

		METHOD int GetDamage(GEntityType Ent);
		METHOD void SetDamage(GEntityType Ent, int Dmg);

		METHOD int GetSplashDamage(GEntityType Ent);
		METHOD void SetSplashDamage(GEntityType Ent, int Dmg);

		METHOD int GetSplashRadius(GEntityType Ent);
		METHOD void SetSplashRadius(GEntityType Ent, int R);

		METHOD MeansOfDeath GetMethodOfDeath(GEntityType Ent);
		METHOD void SetMethodOfDeath(GEntityType Ent, MeansOfDeath M);

		METHOD MeansOfDeath GetSplashMethodOfDeath(GEntityType Ent);
		METHOD void SetSplashMethodOfDeath(GEntityType Ent, MeansOfDeath M);

		METHOD Clipmask GetClipmask(GEntityType Ent);
		METHOD void SetClipmask(GEntityType Ent, Clipmask Mask);

		METHOD GEntityType GetTargetEnt(GEntityType Ent);
		METHOD void SetTargetEnt(GEntityType Ent, GEntityType TEnt);

		METHOD Trajectory GetTrajectory(GEntityType Ent);
		METHOD void SetTrajectory(GEntityType Ent, Trajectory T);

		METHOD Vec3 GetOrigin(GEntityType Ent);
		METHOD void SetOrigin(GEntityType Ent, Vec3 V);

		DeclareFunc(ThinkFunc);
		DeclareFunc(ReachedFunc);
		DeclareFunc(BlockedFunc);
		DeclareFunc(TouchFunc);
		DeclareFunc(UseFunc);
		DeclareFunc(PainFunc);
		DeclareFunc(DieFunc);
	};
}

#undef DeclareFunc
#undef GEntityType
#undef METHOD
#undef DELEGATE