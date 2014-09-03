#pragma once

#define DeclareFunc(T) \
	static T^ Get##T(IntPtr Ent); static void Set##T(IntPtr Ent, T^ Func)

namespace CLIq3 {
	public ref class Entity {
	public:
		static IntPtr Spawn();
		static void Free(IntPtr Ent);

		delegate void ThinkFunc(IntPtr self);
		delegate void ReachedFunc(IntPtr self);
		delegate void BlockedFunc(IntPtr self, IntPtr other);
		delegate void TouchFunc(IntPtr self, IntPtr other, IntPtr trace);
		delegate void UseFunc(IntPtr self, IntPtr other, IntPtr activator);
		delegate void PainFunc(IntPtr self, IntPtr attacker, int damage);
		delegate void DieFunc(IntPtr self, IntPtr inflictor, IntPtr attacker, int damage, int mod);

		static IntPtr GetClient(IntPtr Ent);
		static void SetClient(IntPtr Ent, IntPtr Client);

		static bool GetInUse(IntPtr Ent);
		static void SetInUse(IntPtr Ent, bool B);

		static String^ GetClassname(IntPtr Ent);
		static void SetClassname(IntPtr Ent, String^ S);

		static int GetNextThink(IntPtr Ent);
		static void SetNextThink(IntPtr Ent, int Time);

		static EntType GetEType(IntPtr Ent);
		static void SetEType(IntPtr Ent, EntType T);

		static SVFlags GetSVFlags(IntPtr Ent);
		static void SetSVFlags(IntPtr Ent, SVFlags Flags);

		static Weapon GetWeapon(IntPtr Ent);
		static void SetWeapon(IntPtr Ent, Weapon W);

		static int GetOwnerNum(IntPtr Ent);
		static void SetOwnerNum(IntPtr Ent, int O);

		static IntPtr GetParent(IntPtr Ent);
		static void SetParent(IntPtr Ent, IntPtr Parent);

		static int GetDamage(IntPtr Ent);
		static void SetDamage(IntPtr Ent, int Dmg);

		static int GetSplashDamage(IntPtr Ent);
		static void SetSplashDamage(IntPtr Ent, int Dmg);

		static int GetSplashRadius(IntPtr Ent);
		static void SetSplashRadius(IntPtr Ent, int R);

		static MeansOfDeath GetMethodOfDeath(IntPtr Ent);
		static void SetMethodOfDeath(IntPtr Ent, MeansOfDeath M);

		static MeansOfDeath GetSplashMethodOfDeath(IntPtr Ent);
		static void SetSplashMethodOfDeath(IntPtr Ent, MeansOfDeath M);

		static Clipmask GetClipmask(IntPtr Ent);
		static void SetClipmask(IntPtr Ent, Clipmask Mask);

		static IntPtr GetTargetEnt(IntPtr Ent);
		static void SetTargetEnt(IntPtr Ent, IntPtr TEnt);

		static Trajectory GetTrajectory(IntPtr Ent);
		static void SetTrajectory(IntPtr Ent, Trajectory T);

		static Vec3 GetOrigin(IntPtr Ent);
		static void SetOrigin(IntPtr Ent, Vec3 V);

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