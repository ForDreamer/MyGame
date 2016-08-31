#include "EquipMgr.h"
#include "CharacterMgr.h"
#include <strsafe.h>

Equip::Equip(EquipID id)
:	mEquipId(id),
	mEquiped(false)
{
	switch(id)
	{
		case EQUIP_SLING: strcpy_s(mName, "µ¯¹­"); mEquipType = ET_SHOT; break;
		case EQUIP_SHORTGUN: strcpy_s(mName, "¶ÌÇ¹"); mEquipType = ET_SHOT; break;
		case EQUIP_CLOTH: strcpy_s(mName, "²¼ÒÂ");  mEquipType = ET_CLOTH; break;
		case EQUIP_SHOE: strcpy_s(mName, "²ÝÐ¬"); mEquipType = ET_SHOE; break;
		case EQUIP_GLOVE: strcpy_s(mName, "ÊÖÌ×"); mEquipType = ET_GLOVE; break;
	}
	mCost = sEquipCostList[id];

	for(int i=4; i<EQUIP_NAME_MAX; i++)
	{
		mName[i] = '\0';
	}
}

Equip::~Equip(void)
{
}

void Equip::ApplyEquip(void)
{
	if(mEquiped)
		return;

	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	switch(mEquipId)
	{
		case EQUIP_SLING: pPlayer->SetAttrib(CAB_AT, pPlayer->GetAttrib(CAB_AT)+5); break;
		case EQUIP_SHORTGUN: pPlayer->SetAttrib(CAB_AT, pPlayer->GetAttrib(CAB_AT)+25);break;
		case EQUIP_CLOTH: pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF)+10);break;
		case EQUIP_SHOE: pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF)+5);break;
		case EQUIP_GLOVE: pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF)+3);break;
	}
	mEquiped = true;
}

void Equip::UnApplyEquip(void)
{
	if(!mEquiped)
		return;

	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	switch(mEquipId)
	{
		case EQUIP_SLING: pPlayer->SetAttrib(CAB_AT, pPlayer->GetAttrib(CAB_AT)-5); break;
		case EQUIP_SHORTGUN: pPlayer->SetAttrib(CAB_AT, pPlayer->GetAttrib(CAB_AT)-25);break;
		case EQUIP_CLOTH: pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF)-10);break;
		case EQUIP_SHOE: pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF)-5);break;
		case EQUIP_GLOVE: pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF)-3);break;
	}
	mEquiped = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_SINGLETON(EquipMgr);
static EquipMgr sEquipMgr;

EquipMgr::EquipMgr(void)
:	mEquipNum(0)
{
	for(int i=0; i<MAX_EQUIP_NUM; i++)
	{
		mEquips[i] = NULL;
	}
}

EquipMgr::~EquipMgr(void)
{
	Reset();
}

void EquipMgr::Reset(void)
{
	for(int i=0; i<MAX_EQUIP_NUM; i++)
	{
		if(mEquips[i])
		{
			delete mEquips[i];
			mEquips[i] = NULL;
		}
	}
	mEquipNum = 0;
}

Equip* EquipMgr::AddEquip(EquipID equipId)
{
	if(mEquipNum >= MAX_EQUIP_NUM)
		return NULL;

	assert(mEquips[mEquipNum] == NULL);
	Equip* pEquip = new Equip(equipId);
	mEquips[mEquipNum] = pEquip;
	mEquipNum++;

	return pEquip;
}

void EquipMgr::RemoveEquip(int pos)
{
	assert(pos < mEquipNum);
	assert(mEquips[pos]);

	delete mEquips[pos];
	mEquips[pos] = NULL;
	mEquipNum--;

	for(int i=pos; i<MAX_EQUIP_NUM-1; i++)
	{
		mEquips[i] = mEquips[i+1];
	}
	mEquips[MAX_EQUIP_NUM-1] = NULL; 
}

EquipID EquipMgr::GetAttackWeapon(void)
{
	EquipID retID = EQUIP_INVALID;

	for(int i=0; i<MAX_EQUIP_NUM; i++)
	{
		if(mEquips[i] && mEquips[i]->IsEquiped())
		{
			if(mEquips[i]->GetID() == EQUIP_SLING)
			{
				retID = EQUIP_SLING;
				break;
			}
			else if(mEquips[i]->GetID() == EQUIP_SHORTGUN)
			{
				retID = EQUIP_SHORTGUN;
				break;
			}
		}
	}
	return retID;
}