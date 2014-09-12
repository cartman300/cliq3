#include "../../cgame/cg_local.h"
#include "../header/Draw.h"

#define REFDEF ((refdef_t*)(RefDef).ToPointer())

using namespace CLIq3;

void Draw::DynamicLight(Vec3 Pos) {
	vec3_t P;
	Pos.ToVec3t(P);

	trap_R_AddLightToScene(P, 1.0f, 1, 1, 1);
}