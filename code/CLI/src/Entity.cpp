#include "../../qcommon/q_shared.h"
#include "../../game/g_local.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

#define Protected(body) try body catch(Exception^ E) { CLIq3::sAPI::PrintError(String::Format("{0}\n", E->ToString())); }

#define ToGEntity(P) ((gentity_t*)P.ToPointer())
#define ENT ToGEntity(Ent)

#define ToGClient(P) ((gclient_t*)P.ToPointer())

#define DefineFunc(A, B) \
	Entity::A^ Entity::Get##A(IntPtr Ent) { \
	gentity_t* E = ToGEntity(Ent); if (E == NULL) throw gcnew Exception("Ent was null in Get" #A); \
	if (E->B == NULL) throw gcnew Exception("Ent->" #B " was null in Get" #A); \
	return (Entity::A^)Marshal::GetDelegateForFunctionPointer(IntPtr(E->B), Entity::A::typeid); } \
	void Entity::Set##A(IntPtr Ent, Entity::A^ F) { \
	ToGEntity(Ent)->B = (ent##A)Marshal::GetFunctionPointerForDelegate(F).ToPointer(); }

DefineFunc(ThinkFunc, think);
DefineFunc(ReachedFunc, reached);
DefineFunc(BlockedFunc, blocked);
DefineFunc(TouchFunc, touch);
DefineFunc(UseFunc, use);
DefineFunc(PainFunc, pain);
DefineFunc(DieFunc, die);

#undef DefineFunc

IntPtr Entity::Spawn() {
	return IntPtr(G_Spawn());
}

void Entity::Free(IntPtr Ent) {
	G_FreeEntity(ENT);
}

IntPtr Entity::GetClient(IntPtr Ent) {
	return IntPtr(ENT->client);
}
void Entity::SetClient(IntPtr Ent, IntPtr Client) {
	ENT->client = ToGClient(Client);
}

bool Entity::GetInUse(IntPtr Ent) {
	return (bool)ENT->inuse;
}
void Entity::SetInUse(IntPtr Ent, bool B) {
	ENT->inuse = (qboolean)B;
}

String^ Entity::GetClassname(IntPtr Ent) {
	return gcnew String(ToGEntity(Ent)->classname);
}
void Entity::SetClassname(IntPtr Ent, String^ S) {
	ENT->classname = (char*)CreateNString(S);
}

int Entity::GetNextThink(IntPtr Ent) {
	return ENT->nextthink;
}
void Entity::SetNextThink(IntPtr Ent, int Time) {
	ENT->nextthink = Time;
}

EntType Entity::GetEType(IntPtr Ent) {
	return (EntType)ENT->s.eType;
}
void Entity::SetEType(IntPtr Ent, EntType T) {
	ENT->s.eType = (int)T;
}

SVFlags Entity::GetSVFlags(IntPtr Ent) {
	return (SVFlags)ENT->r.svFlags;
}
void Entity::SetSVFlags(IntPtr Ent, SVFlags Flags) {
	ENT->r.svFlags = (int)Flags;
}

Weapon Entity::GetWeapon(IntPtr Ent) {
	return (Weapon)ENT->s.weapon;
}
void Entity::SetWeapon(IntPtr Ent, Weapon W) {
	ENT->s.weapon = (int)W;
}

int Entity::GetOwnerNum(IntPtr Ent) {
	return ENT->r.ownerNum;
}
void Entity::SetOwnerNum(IntPtr Ent, int O) {
	ENT->r.ownerNum = O;
}

IntPtr Entity::GetParent(IntPtr Ent) {
	return IntPtr(ENT->parent);
}
void Entity::SetParent(IntPtr Ent, IntPtr Parent) {
	ENT->parent = ToGEntity(Parent);
}

int Entity::GetDamage(IntPtr Ent) {
	return ENT->damage;
}
void Entity::SetDamage(IntPtr Ent, int Dmg) {
	ENT->damage = Dmg;
}

int Entity::GetSplashDamage(IntPtr Ent) {
	return ENT->splashDamage;
}
void Entity::SetSplashDamage(IntPtr Ent, int Dmg) {
	ENT->splashDamage = Dmg;
}

int Entity::GetSplashRadius(IntPtr Ent) {
	return ENT->splashRadius;
}
void Entity::SetSplashRadius(IntPtr Ent, int R) {
	ENT->splashRadius = R;
}

MeansOfDeath Entity::GetMethodOfDeath(IntPtr Ent) {
	return (MeansOfDeath)ENT->methodOfDeath;
}
void Entity::SetMethodOfDeath(IntPtr Ent, MeansOfDeath M) {
	ENT->methodOfDeath = (meansOfDeath_t)M;
}

MeansOfDeath Entity::GetSplashMethodOfDeath(IntPtr Ent) {
	return (MeansOfDeath)ENT->splashMethodOfDeath;
}
void Entity::SetSplashMethodOfDeath(IntPtr Ent, MeansOfDeath M) {
	ENT->splashMethodOfDeath = (meansOfDeath_t)M;
}

Clipmask Entity::GetClipmask(IntPtr Ent) {
	return (Clipmask)ENT->clipmask;
}
void Entity::SetClipmask(IntPtr Ent, Clipmask Mask) {
	ENT->clipmask = (int)Mask;
}

IntPtr Entity::GetTargetEnt(IntPtr Ent) {
	return IntPtr(ENT->target_ent);
}
void Entity::SetTargetEnt(IntPtr Ent, IntPtr TEnt) {
	ENT->target_ent = ToGEntity(TEnt);
}

Trajectory Entity::GetTrajectory(IntPtr Ent) {
	return Trajectory::FromTrajectoryt(&ENT->s.pos);
}
void Entity::SetTrajectory(IntPtr Ent, Trajectory T) {
	T.ToTrajectoryt(&ENT->s.pos);
}

Vec3 Entity::GetOrigin(IntPtr Ent) {
	return Vec3::FromVec3t(ENT->r.currentOrigin);
}
void Entity::SetOrigin(IntPtr Ent, Vec3 Vec) {
	Vec.ToVec3t(ENT->r.currentOrigin);
}