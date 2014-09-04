#include "../../qcommon/q_shared.h"
#include "../../game/g_local.h"

#include "../header/Entity.h"

using namespace CLIq3;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;
using namespace System::Runtime::CompilerServices;

#define Protected(body) try body catch(Exception^ E) { CLIq3::sAPI::PrintError(String::Format("{0}\n", E->ToString())); }

#define GEntityType EntPtr^
#define ToGEntity(P) ((gentity_t*)P->Ptr)
#define ENT ToGEntity(Ent)

#define ToGClient(P) ((gclient_t*)P->Ptr)

#define DefineFunc(A, B) \
	A^ Entity::Get##A(EntPtr^ Ent) { \
		if (ENT == NULL) \
			throw gcnew Exception("Ent was null in Get" #A); \
		if (ENT->B == NULL) \
			/*throw gcnew Exception("Ent->" #B " was null in Get" #A);*/ return nullptr; \
		return (A^)Marshal::GetDelegateForFunctionPointer(IntPtr(ENT->B), A::typeid); \
	} \
	void Entity::Set##A(EntPtr^ Ent, A^ F) { \
		ENT->B = (ent##A)Marshal::GetFunctionPointerForDelegate(F).ToPointer(); \
	}

DefineFunc(ThinkFunc, think);
DefineFunc(ReachedFunc, reached);
DefineFunc(BlockedFunc, blocked);
DefineFunc(TouchFunc, touch);
DefineFunc(UseFunc, use);
DefineFunc(PainFunc, pain);
DefineFunc(DieFunc, die);

#undef DefineFunc

GEntityType Entity::Spawn() {
	return gcnew EntPtr(G_Spawn());
}

void Entity::Free(GEntityType Ent) {
	G_FreeEntity(ENT);
}

ClientPtr^ Entity::GetClient(GEntityType Ent) {
	return gcnew ClientPtr(ENT->client);
}
void Entity::SetClient(GEntityType Ent, ClientPtr^ Client) {
	ENT->client = ToGClient(Client);
}

bool Entity::GetInUse(GEntityType Ent) {
	return (bool)ENT->inuse;
}
void Entity::SetInUse(GEntityType Ent, bool B) {
	ENT->inuse = (qboolean)B;
}

String^ Entity::GetClassname(GEntityType Ent) {
	return gcnew String(ToGEntity(Ent)->classname);
}
void Entity::SetClassname(GEntityType Ent, String^ S) {
	ENT->classname = (char*)CreateNString(S);
}

int Entity::GetNextThink(GEntityType Ent) {
	return ENT->nextthink;
}
void Entity::SetNextThink(GEntityType Ent, int Time) {
	ENT->nextthink = Time;
}

EntType Entity::GetEType(GEntityType Ent) {
	return (EntType)ENT->s.eType;
}
void Entity::SetEType(GEntityType Ent, EntType T) {
	ENT->s.eType = (int)T;
}

SVFlags Entity::GetSVFlags(GEntityType Ent) {
	return (SVFlags)ENT->r.svFlags;
}
void Entity::SetSVFlags(GEntityType Ent, SVFlags Flags) {
	ENT->r.svFlags = (int)Flags;
}

Weapon Entity::GetWeapon(GEntityType Ent) {
	return (Weapon)ENT->s.weapon;
}
void Entity::SetWeapon(GEntityType Ent, Weapon W) {
	ENT->s.weapon = (int)W;
}

int Entity::GetOwnerNum(GEntityType Ent) {
	return ENT->r.ownerNum;
}
void Entity::SetOwnerNum(GEntityType Ent, int O) {
	ENT->r.ownerNum = O;
}

GEntityType Entity::GetParent(GEntityType Ent) {
	return gcnew EntPtr(ENT->parent);
}
void Entity::SetParent(GEntityType Ent, GEntityType Parent) {
	ENT->parent = ToGEntity(Parent);
}

int Entity::GetDamage(GEntityType Ent) {
	return ENT->damage;
}
void Entity::SetDamage(GEntityType Ent, int Dmg) {
	ENT->damage = Dmg;
}

int Entity::GetSplashDamage(GEntityType Ent) {
	return ENT->splashDamage;
}
void Entity::SetSplashDamage(GEntityType Ent, int Dmg) {
	ENT->splashDamage = Dmg;
}

int Entity::GetSplashRadius(GEntityType Ent) {
	return ENT->splashRadius;
}
void Entity::SetSplashRadius(GEntityType Ent, int R) {
	ENT->splashRadius = R;
}

MeansOfDeath Entity::GetMethodOfDeath(GEntityType Ent) {
	return (MeansOfDeath)ENT->methodOfDeath;
}
void Entity::SetMethodOfDeath(GEntityType Ent, MeansOfDeath M) {
	ENT->methodOfDeath = (meansOfDeath_t)M;
}

MeansOfDeath Entity::GetSplashMethodOfDeath(GEntityType Ent) {
	return (MeansOfDeath)ENT->splashMethodOfDeath;
}
void Entity::SetSplashMethodOfDeath(GEntityType Ent, MeansOfDeath M) {
	ENT->splashMethodOfDeath = (meansOfDeath_t)M;
}

Clipmask Entity::GetClipmask(GEntityType Ent) {
	return (Clipmask)ENT->clipmask;
}
void Entity::SetClipmask(GEntityType Ent, Clipmask Mask) {
	ENT->clipmask = (int)Mask;
}

GEntityType Entity::GetTargetEnt(GEntityType Ent) {
	return gcnew EntPtr(ENT->target_ent);
}
void Entity::SetTargetEnt(GEntityType Ent, GEntityType TEnt) {
	ENT->target_ent = ToGEntity(TEnt);
}

Trajectory Entity::GetTrajectory(GEntityType Ent) {
	return Trajectory::FromTrajectoryt(&ENT->s.pos);
}
void Entity::SetTrajectory(GEntityType Ent, Trajectory T) {
	T.ToTrajectoryt(&ENT->s.pos);
}

Vec3 Entity::GetOrigin(GEntityType Ent) {
	return Vec3::FromVec3t(ENT->r.currentOrigin);
}
void Entity::SetOrigin(GEntityType Ent, Vec3 Vec) {
	Vec.ToVec3t(ENT->r.currentOrigin);
}