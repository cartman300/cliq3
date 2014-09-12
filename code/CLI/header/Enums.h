#pragma once

#ifndef SERVER
#error Must compile under SERVER
#endif

namespace CLIq3 {
	public enum class MeansOfDeath : int {
		MOD_UNKNOWN,
		MOD_SHOTGUN,
		MOD_GAUNTLET,
		MOD_MACHINEGUN,
		MOD_GRENADE,
		MOD_GRENADE_SPLASH,
		MOD_ROCKET,
		MOD_ROCKET_SPLASH,
		MOD_PLASMA,
		MOD_PLASMA_SPLASH,
		MOD_RAILGUN,
		MOD_LIGHTNING,
		MOD_BFG,
		MOD_BFG_SPLASH,
		MOD_WATER,
		MOD_SLIME,
		MOD_LAVA,
		MOD_CRUSH,
		MOD_TELEFRAG,
		MOD_FALLING,
		MOD_SUICIDE,
		MOD_TARGET_LASER,
		MOD_TRIGGER_HURT,
	#ifdef MISSIONPACK
		MOD_NAIL,
		MOD_CHAINGUN,
		MOD_PROXIMITY_MINE,
		MOD_KAMIKAZE,
		MOD_JUICED,
	#endif
		MOD_GRAPPLE
	};

	public enum class Weapon : int {
		WP_NONE,

		WP_GAUNTLET,
		WP_MACHINEGUN,
		WP_SHOTGUN,
		WP_GRENADE_LAUNCHER,
		WP_ROCKET_LAUNCHER,
		WP_LIGHTNING,
		WP_RAILGUN,
		WP_PLASMAGUN,
		WP_BFG,
		WP_GRAPPLING_HOOK,
	#ifdef MISSIONPACK
		WP_NAILGUN,
		WP_PROX_LAUNCHER,
		WP_CHAINGUN,
	#endif

		WP_NUM_WEAPONS
	};

	public enum class EntType : int {
		ET_GENERAL,
		ET_PLAYER,
		ET_ITEM,
		ET_MISSILE,
		ET_MOVER,
		ET_BEAM,
		ET_PORTAL,
		ET_SPEAKER,
		ET_PUSH_TRIGGER,
		ET_TELEPORT_TRIGGER,
		ET_INVISIBLE,
		ET_GRAPPLE,
		ET_TEAM,
		ET_EVENTS
	};

	public enum class TrajectoryType : int {
		TR_STATIONARY,
		TR_INTERPOLATE,				// non-parametric, but interpolate between snapshots
		TR_LINEAR,
		TR_LINEAR_STOP,
		TR_SINE,					// value = base + sin( time / duration ) * delta
		TR_GRAVITY
	};

	public enum class SVFlags : int {
		CLIENTMASK = 0x00000002,
		BOT = 0x00000008,
		BROADCAST = 0x00000020,
		PORTAL = 0x00000040,
		USE_CURRENT_ORIGIN = 0x00000080,
		SINGLECLIENT = 0x00000100,
		NOSERVERINFO = 0x00000200,
		CAPSULE = 0x00000400,
		NOTSINGLECLIENT = 0x00000800		
	};

	public enum class Clipmask : int {
		ALL = (-1),
		SOLID = (CONTENTS_SOLID),
		PLAYERSOLID = (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY),
		DEADSOLID = (CONTENTS_SOLID|CONTENTS_PLAYERCLIP),
		WATER = (CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME),
		OPAQUE = (CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA),
		SHOT = (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)
	};

	public value class Vec3 {
	public:
		float X; float Y; float Z;

		void Snap();
		Vec3 Perpendicular();
		Vec3 Cross(Vec3 Vec);
		float Normalize();

	internal:
		void ToVec3t(vec3_t Vec);
		static Vec3 FromVec3t(vec3_t Vec);
	};

	public value class Trajectory {
	public:
		TrajectoryType Type;
		int Time;
		int Duration;
		Vec3 Base;
		Vec3 Delta;			// velocity, etc

	internal:
		void ToTrajectoryt(trajectory_t* T);
		static Trajectory FromTrajectoryt(trajectory_t* T);
	};
}