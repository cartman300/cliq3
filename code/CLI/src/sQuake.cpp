#include "../../qcommon/q_shared.h"
#include "../../game/g_local.h"

#include "../header/sQuake.h"

using namespace CLIq3;

EntPtr^ sQuake::Fire_plasma(EntPtr^ Parent, Vec3 Start, Vec3 Dir) {
	vec3_t _Start;
	Start.ToVec3t(_Start);
	vec3_t _Dir;
	Dir.ToVec3t(_Dir);
	return gcnew EntPtr(fire_plasma((gentity_t*)Parent->Ptr, _Start, _Dir));
}

EntPtr^ sQuake::Fire_grenade(EntPtr^ Parent, Vec3 Start, Vec3 Dir) {
	vec3_t _Start;
	Start.ToVec3t(_Start);
	vec3_t _Dir;
	Dir.ToVec3t(_Dir);
	return gcnew EntPtr(fire_grenade((gentity_t*)Parent->Ptr, _Start, _Dir));
}

EntPtr^ sQuake::Fire_bfg(EntPtr^ Parent, Vec3 Start, Vec3 Dir) {
	vec3_t _Start;
	Start.ToVec3t(_Start);
	vec3_t _Dir;
	Dir.ToVec3t(_Dir);
	return gcnew EntPtr(fire_bfg((gentity_t*)Parent->Ptr, _Start, _Dir));
}

EntPtr^ sQuake::Fire_rocket(EntPtr^ Parent, Vec3 Start, Vec3 Dir) {
	vec3_t _Start;
	Start.ToVec3t(_Start);
	vec3_t _Dir;
	Dir.ToVec3t(_Dir);
	return gcnew EntPtr(fire_rocket((gentity_t*)Parent->Ptr, _Start, _Dir));
}

EntPtr^ sQuake::Fire_grapple(EntPtr^ Parent, Vec3 Start, Vec3 Dir) {
	vec3_t _Start;
	Start.ToVec3t(_Start);
	vec3_t _Dir;
	Dir.ToVec3t(_Dir);
	return gcnew EntPtr(fire_grapple((gentity_t*)Parent->Ptr, _Start, _Dir));
}

void sQuake::Fire_bullet(EntPtr^ Parent, float Spread, int Damage, MeansOfDeath Mod, Vec3 Start, Vec3 Dir) {
	vec3_t _Start;
	Start.ToVec3t(_Start);
	vec3_t _Dir;
	Dir.ToVec3t(_Dir);
	fire_bullet((gentity_t*)Parent->Ptr, Spread, Damage, (int)Mod, _Start, _Dir);
}