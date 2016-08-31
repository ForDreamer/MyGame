#ifndef _BattleActor_H_
#define _BattleActor_H_

#include "../../Engine/Hud/Sprite.h"

#define MAX_ENEMY_TYPE 7
#define MAX_BATACTOR_NAME 16

struct BatActorAttrib
{
	char Name[MAX_BATACTOR_NAME];
	short HP;
	short HPMax;
	short AT;
	short DF;
	short Speed;
	short Exp;
	short Gold;
	short Type;
};

enum WeaponType
{
	WT_NONE = -1,
	WT_BODY = 0,
	WT_SLING = 1,
	WT_GUN = 2,
	WT_CANNON = 3,
	WT_FIRE = 4,
	WT_BOMB = 5
};

class BattleActor
{
public:
	BattleActor(void);
	~BattleActor(void);

	void Init(int texId, int enemyId, float posX, float posY, float sizeX, float sizeY);
	void Update(void);
	void Render(void);

	void SetVisible(bool visible) { mSprActor.SetVisible(visible); }
	bool IsVisible(void) { return mSprActor.GetVisible(); }
	float GetPosX(void) { return mSprActor.GetPositionX(); }
	float GetPosY(void) { return mSprActor.GetPositionY(); }

	void SetupBatAttribs(int enemyId, char nameFix);
	int GetEnemyId(void) { return mEnemyId; }
	int GetTexId(void) { return mTexId; }
	void SetDieBlink(bool blink) { mbDieBlink = blink; }
	WeaponType GetWeaponType(void) { return (WeaponType)mAttrib.Type; }
	void SetAttackBlink(bool blink);
	void SetBeenDamaged(bool damaged);

	char* GetName(void) { return mAttrib.Name; }
	void AddHP(int add) { mAttrib.HP += add; if(mAttrib.HP > mAttrib.HPMax) mAttrib.HP = mAttrib.HPMax; }
	void MinusHP(int minus) { mAttrib.HP -= minus; if(mAttrib.HP < 0) mAttrib.HP = 0; }
	bool IsMaxHP(void) { return mAttrib.HP == mAttrib.HPMax;}

	short GetSP(void) { return mSP; }
	void AddSP(int add) { mSP += add; if(mSP > mSPMax) mSP = mSPMax; }
	void MinusSP(int minus) { mSP -= minus; if(mSP < 0) mSP = 0; }

	short GetHP(void) { return mAttrib.HP; }
	short GetHPMax(void) { return mAttrib.HPMax; }
	short GetAT(void) { return mAttrib.AT; }
	short GetDF(void) { return mAttrib.DF; }
	short GetSpeed(void) { return mAttrib.Speed; }
	short GetExp(void) { return mAttrib.Exp; }
	short GetGold(void) { return mAttrib.Gold; }

	bool IsPlayer(void) { return mEnemyId == -1; }
	void RefreshTexIdByMode(void);
	void SetSprTexture(int texId);
	void ResetSprTexture(void);
	WeaponType GetWeaponTypeByEquip(void);
	
	void SetGuarding(bool guard) { mbIsGuarding = guard; }
	bool IsGuarding(void) { return mbIsGuarding; }

private:
	Sprite mSprActor;
	BatActorAttrib mAttrib;
	int mEnemyId;
	int mTexId;
	bool mbIsGuarding;
	bool mbDieBlink;
	float mbBlinkTimer;
	bool mbAttackBlink;
	float mbAttackBlinkTimer;
	int mbAttackBlinkCount;
	bool mbDamaged;
	float mbDamageTimer;

	short mSP;
	short mSPMax;
};

#endif