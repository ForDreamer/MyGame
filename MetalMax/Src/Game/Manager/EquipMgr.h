#ifndef _EquipMgr_H_
#define _EquipMgr_H_

#include "../../Engine/Misc/Singleton.h"

enum EquipID
{
	EQUIP_INVALID = -1,
	EQUIP_SLING = 0,
	EQUIP_SHORTGUN = 1,
	EQUIP_CLOTH = 2,
	EQUIP_SHOE = 3,
	EQUIP_GLOVE = 4,
	EQUIP_MAX
};

enum EquipType
{
	ET_SHOT = 0,
	ET_CLOTH = 1,
	ET_SHOE = 2,
	ET_GLOVE = 3,
	ET_MAX
};

static int sEquipCostList[EQUIP_MAX] = {0,120,40,30,15};

#define EQUIP_NAME_MAX 8
class Equip
{
public:
	Equip(EquipID id);
	~Equip(void);

	EquipID GetID(void) { return mEquipId; }
	EquipType GetType(void) { return mEquipType; }
	char* GetName(void) { return mName; }
	int GetCost(void) { return mCost; }

	void ApplyEquip(void);
	void UnApplyEquip(void);
	bool IsEquiped(void) { return mEquiped; }
	void SetEquiped(void) { mEquiped = true; }

private:
	EquipID mEquipId;
	EquipType mEquipType;
	char mName[EQUIP_NAME_MAX];
	int mCost;
	bool mEquiped;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_EQUIP_NUM 8
class EquipMgr: public Singleton<EquipMgr>
{
public:
	EquipMgr(void);
	~EquipMgr(void);

	void Reset(void);

	bool IsEquipFull(void) { return mEquipNum == MAX_EQUIP_NUM; }
	Equip* GetEquip(int pos) { return mEquips[pos]; }
	int GetEquipNum(void) { return mEquipNum; }

	EquipID GetAttackWeapon(void);
	
	Equip* AddEquip(EquipID itemId);
	void RemoveEquip(int pos);
	
private:
	Equip* mEquips[MAX_EQUIP_NUM];
	int mEquipNum;
};
#define gEquipMgr EquipMgr::GetInstance()

#endif