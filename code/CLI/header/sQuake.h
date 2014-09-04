#pragma once

namespace CLIq3 {
	public ref class sQuake {
	public:	
		static EntPtr^ Fire_plasma(EntPtr^ Parent, Vec3 Start, Vec3 Dir);
		static EntPtr^ Fire_grenade(EntPtr^ Parent, Vec3 Start, Vec3 Dir);
		static EntPtr^ Fire_bfg(EntPtr^ Parent, Vec3 Start, Vec3 Dir);
		static EntPtr^ Fire_rocket(EntPtr^ Parent, Vec3 Start, Vec3 Dir);
		static EntPtr^ Fire_grapple(EntPtr^ Parent, Vec3 Start, Vec3 Dir);
	};
}