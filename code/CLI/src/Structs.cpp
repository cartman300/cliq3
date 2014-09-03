#include "../../qcommon/q_shared.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

void Vec3::Snap() {
	X = (int)X;
	Y = (int)Y;
	Z = (int)Z;
}

void Vec3::ToVec3t(vec3_t Vec) {
	Vec[0] = X;
	Vec[1] = Y;
	Vec[2] = Z;
}

Vec3 Vec3::FromVec3t(vec3_t Vec) {
	Vec3 V;
	V.X = Vec[0];
	V.Y = Vec[1];
	V.Z = Vec[2];
	return V;
}

void Trajectory::ToTrajectoryt(trajectory_t* T) {
	Base.ToVec3t(T->trBase);
	Delta.ToVec3t(T->trDelta);
	T->trDuration = Duration;
	T->trTime = Time;
	T->trType = (trType_t)this->Type;
}

Trajectory Trajectory::FromTrajectoryt(trajectory_t* T) {
	Trajectory Tr;
	Tr.Base = Vec3::FromVec3t(T->trBase);
	Tr.Delta = Vec3::FromVec3t(T->trDelta);
	Tr.Duration = T->trDuration;
	Tr.Time = T->trTime;
	Tr.Type = (TrajectoryType)T->trType;
	return Tr;
}