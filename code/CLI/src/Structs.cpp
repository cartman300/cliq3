#include "../../qcommon/q_shared.h"
#include "../header/Enums.h"

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

Vec3 Vec3::Perpendicular() {
	vec3_t A, B;
	ToVec3t(A);
	PerpendicularVector(B, A);
	return Vec3::FromVec3t(B);
}

Vec3 Vec3::Cross(Vec3 Vec) {
	vec3_t A, B, C;
	ToVec3t(A);
	Vec.ToVec3t(B);
	CrossProduct(A, B, C);
	return Vec3::FromVec3t(C);
}

float Vec3::Normalize() {
	vec3_t V;
	ToVec3t(V);
	float Len = VectorNormalize(V);
	X = V[0];
	Y = V[1];
	Z = V[2];
	return Len;
}

void Trajectory::ToTrajectoryt(trajectory_t* T) {
	Base.ToVec3t(T->trBase);
	Delta.ToVec3t(T->trDelta);
	T->trDuration = Duration;
	T->trTime = Time;
	T->trType = (trType_t)(int)this->Type;
}

Trajectory Trajectory::FromTrajectoryt(trajectory_t* T) {
	Trajectory Tr;
	Tr.Base = Vec3::FromVec3t(T->trBase);
	Tr.Delta = Vec3::FromVec3t(T->trDelta);
	Tr.Duration = T->trDuration;
	Tr.Time = T->trTime;
	Tr.Type = (TrajectoryType)(int)T->trType;
	return Tr;
}