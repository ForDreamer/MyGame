#include "SaveLoadMgr.h"
#include "../../Main/Global.h"
#include "../MapEngine/MapManager.h"
#include "CharacterMgr.h"
#include "BattleTankMgr.h"
#include "GameStatusMgr.h"
#include "ItemMgr.h"
#include "EquipMgr.h"

DECLARE_SINGLETON(SaveLoadMgr);
static SaveLoadMgr sSaveLoadMgr;

void SLMapInfo::SaveFromGame(void)
{
	gMapMgr.GetActPlayerInfo(playerMapId, playerTileX, playerTileY, playerDir);
	gMapMgr.GetActTankInfo(tankMapId, tankTileX, tankTileY, tankDir);
}

void SLMapInfo::LoadToGame(void)
{
	gMapMgr.SetNextMapInfo((MapID)playerMapId, playerTileX, playerTileY, playerDir);
	gMapMgr.SetTankMapInfo((MapID)tankMapId, tankTileX, tankTileY, tankDir);
}

bool SLMapInfo::Save(FILE* fp)
{
	fprintf(fp, "#PlayerMapInfo: %d %d %d %d\n", playerMapId, playerTileX, playerTileY, playerDir);
	fprintf(fp, "#TankMapInfo: %d %d %d %d\n", tankMapId, tankTileX, tankTileY, tankDir);
	return true;
}

bool SLMapInfo::Load(FILE* fp)
{
	int ret = 0;
	ret = fscanf(fp, "#PlayerMapInfo: %d%d%d%d\n", &playerMapId, &playerTileX, &playerTileY, &playerDir);
	if(ret <= 0)
		return false;

	ret = fscanf(fp, "#TankMapInfo: %d%d%d%d\n", &tankMapId, &tankTileX, &tankTileY, &tankDir);
	if(ret <= 0)
		return false;

	return true;
}



void SLCharInfo::SaveFromGame(void)
{
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	strcpy_s(name, pPlayer->GetName());
	HP = pPlayer->GetAttrib(CAB_HP);
	HPMax = pPlayer->GetAttrib(CAB_HPMAX);
	AT = pPlayer->GetAttrib(CAB_AT);
	DF = pPlayer->GetAttrib(CAB_DF);
	STR = pPlayer->GetAttrib(CAB_STR);
	INT = pPlayer->GetAttrib(CAB_INT);
	AGL = pPlayer->GetAttrib(CAB_AGL);
	PHY = pPlayer->GetAttrib(CAB_PHY);
	LV = pPlayer->GetAttrib(CAB_LV);
	EXP = pPlayer->GetAttrib(CAB_EXP);
}

void SLCharInfo::LoadToGame(void)
{
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	pPlayer->SetName(name);
	pPlayer->SetAttrib(CAB_HP, HP);
	pPlayer->SetAttrib(CAB_HPMAX, HPMax);
	pPlayer->SetAttrib(CAB_AT, AT);
	pPlayer->SetAttrib(CAB_DF, DF);
	pPlayer->SetAttrib(CAB_STR, STR);
	pPlayer->SetAttrib(CAB_INT, INT);
	pPlayer->SetAttrib(CAB_AGL, AGL);
	pPlayer->SetAttrib(CAB_PHY, PHY);
	pPlayer->SetAttrib(CAB_LV, LV);
	pPlayer->SetAttrib(CAB_EXP, EXP);
}

bool SLCharInfo::Save(FILE* fp)
{
	fprintf(fp, "#PlayerInfo: %s %d %d %d %d %d %d %d %d %d %d\n", name, HP, HPMax, AT, DF, STR, INT, AGL, PHY, LV, EXP);
	return true;
}

bool SLCharInfo::Load(FILE* fp)
{
	int ret = 0;
	ret = fscanf(fp, "#PlayerInfo: %s%d%d%d%d%d%d%d%d%d%d\n", name, &HP, &HPMax, &AT, &DF, &STR, &INT, &AGL, &PHY, &LV, &EXP);
	if(ret <= 0)
		return false;

	return true;
}



void SLTankInfo::SaveFromGame(void)
{
	BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
	strcpy_s(name, pTank->GetName());
	SP = pTank->GetSP();
	ST = pTank->GetST();
}

void SLTankInfo::LoadToGame(void)
{
	BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
	pTank->SetName(name);
	pTank->SetSP(SP);
	pTank->SetST(ST);
}

bool SLTankInfo::Save(FILE* fp)
{
	fprintf(fp, "#TankInfo: %s %d %d\n", name, SP, ST);
	return true;
}

bool SLTankInfo::Load(FILE* fp)
{
	int ret = 0;
	ret = fscanf(fp, "#TankInfo: %s%d%d\n", name, &SP, &ST);
	if(ret <= 0)
		return false;

	return true;
}



void SLStatusInfo::SaveFromGame(void)
{
	state = gGameStatusMgr.GetGameState();
	gold = gGameStatusMgr.GetGoldNum();
	for(int i=0; i<ITEM_UNLOCK_MAX; i++)
	{
		searchUnlocks[i] = gGameStatusMgr.IsItemUnlocked(i);
	}
}

void SLStatusInfo::LoadToGame(void)
{
	gGameStatusMgr.SetGameState((GameState)state);
	gGameStatusMgr.SetGoldNum(gold);
	for(int i=0; i<ITEM_UNLOCK_MAX; i++)
	{
		if(searchUnlocks[i])
			gGameStatusMgr.SetItemUnlocked(i);
	}
}

bool SLStatusInfo::Save(FILE* fp)
{
	fprintf(fp, "#StatusInfo: %d %d %d %d %d %d %d %d %d %d\n", state, gold,
		searchUnlocks[0], searchUnlocks[1], searchUnlocks[2], searchUnlocks[3],
		searchUnlocks[4], searchUnlocks[5], searchUnlocks[6], searchUnlocks[7]);
	return true;
}

bool SLStatusInfo::Load(FILE* fp)
{
	int ret = 0;
	ret = fscanf(fp, "#StatusInfo: %d%d%d%d%d%d%d%d%d%d\n", &state, &gold,
		&searchUnlocks[0], &searchUnlocks[1], &searchUnlocks[2], &searchUnlocks[3],
		&searchUnlocks[4], &searchUnlocks[5], &searchUnlocks[6], &searchUnlocks[7]);
	if(ret <= 0)
		return false;

	return true;
}



void SLItemInfo::SaveFromGame(void)
{
	for(int i=0; i<MAX_ITEM_NUM; i++)
	{
		Item* pItem = gItemMgr.GetItem(i);
		if(pItem)
			itemIDs[i] = pItem->GetID();
		else
			itemIDs[i] = -1;
	}
}

void SLItemInfo::LoadToGame(void)
{
	for(int i=0; i<MAX_ITEM_NUM; i++)
	{
		if(itemIDs[i] >= 0)
		{
			gItemMgr.AddItem((ItemID)itemIDs[i]);
		}
	}
}

bool SLItemInfo::Save(FILE* fp)
{
	fprintf(fp, "#ItemInfo: %d %d %d %d %d %d %d %d\n",
		itemIDs[0], itemIDs[1], itemIDs[2], itemIDs[3],
		itemIDs[4], itemIDs[5], itemIDs[6], itemIDs[7]);
	return true;
}

bool SLItemInfo::Load(FILE* fp)
{
	int ret = 0;
	ret = fscanf(fp, "#ItemInfo: %d%d%d%d%d%d%d%d\n",
		&itemIDs[0], &itemIDs[1], &itemIDs[2], &itemIDs[3],
		&itemIDs[4], &itemIDs[5], &itemIDs[6], &itemIDs[7]);
	if(ret <= 0)
		return false;

	return true;
}



void SLEquipInfo::SaveFromGame(void)
{
	for(int i=0; i<MAX_EQUIP_NUM; i++)
	{
		Equip* pEquip = gEquipMgr.GetEquip(i);
		if(pEquip)
		{
			equipIDs[i] = pEquip->GetID();
			equiped[i] = pEquip->IsEquiped();
		}
		else
		{
			equipIDs[i] = -1;
			equiped[i] = false;
		}
	}
}

void SLEquipInfo::LoadToGame(void)
{
	for(int i=0; i<MAX_EQUIP_NUM; i++)
	{
		if(equipIDs[i] >= 0)
		{
			Equip* pEquip = gEquipMgr.AddEquip((EquipID)equipIDs[i]);
			if(equiped[i])
				pEquip->SetEquiped();
		}
	}
}

bool SLEquipInfo::Save(FILE* fp)
{
	fprintf(fp, "#EquipInfo: %d %d %d %d %d %d %d %d\n",
		equipIDs[0], equipIDs[1], equipIDs[2], equipIDs[3],
		equipIDs[4], equipIDs[5], equipIDs[6], equipIDs[7]);
	fprintf(fp, "#Equiped: %d %d %d %d %d %d %d %d\n",
		equiped[0], equiped[1], equiped[2], equiped[3],
		equiped[4], equiped[5], equiped[6], equiped[7]);
	return true;
}

bool SLEquipInfo::Load(FILE* fp)
{
	int ret = 0;
	ret = fscanf(fp, "#EquipInfo: %d %d %d %d %d %d %d %d\n",
		&equipIDs[0], &equipIDs[1], &equipIDs[2], &equipIDs[3],
		&equipIDs[4], &equipIDs[5], &equipIDs[6], &equipIDs[7]);
	if(ret <= 0)
		return false;

	ret = fscanf(fp, "#Equiped: %d%d%d%d%d%d%d%d\n",
		&equiped[0], &equiped[1], &equiped[2], &equiped[3],
		&equiped[4], &equiped[5], &equiped[6], &equiped[7]);
	if(ret <= 0)
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SaveLoadMgr::SaveLoadMgr(void)
{
	int size = sizeof(SaveData);
	ResetSaveData();
}

SaveLoadMgr::~SaveLoadMgr(void)
{
}

bool SaveLoadMgr::Save(void)
{
	SaveSaveData_FromGame();
	SaveSingleFile(sSaveFilename);
	return true;
}

bool SaveLoadMgr::Load(void)
{
	bool ret = LoadSingleFile(sSaveFilename);
	if(!ret)
	{
		ResetSaveData();
		SaveSingleFile(sSaveFilename);
	}
	LoadSaveData_ToGame();
	return true;
}


bool SaveLoadMgr::SaveSingleFile(const char* filename)
{
	FILE* fp = fopen(filename, "wt") ;
	if(!fp)
		return false;

	mSaveData.mapInfo.Save(fp);
	mSaveData.charInfo.Save(fp);
	mSaveData.tankInfo.Save(fp);
	mSaveData.statusInfo.Save(fp);
	mSaveData.itemInfo.Save(fp);
	mSaveData.equipInfo.Save(fp);

	fclose(fp);
	return true;
}

bool SaveLoadMgr::LoadSingleFile(const char* filename)
{
	FILE* fp = fopen(filename, "rt") ;
	if(!fp)
		return false;

	bool ret;
	ret = mSaveData.mapInfo.Load(fp);
	if(!ret) { fclose(fp); return false; }

	ret = mSaveData.charInfo.Load(fp);
	if(!ret) { fclose(fp); return false; }

	ret = mSaveData.tankInfo.Load(fp);
	if(!ret) { fclose(fp); return false; }

	ret = mSaveData.statusInfo.Load(fp);
	if(!ret) { fclose(fp); return false; }

	ret = mSaveData.itemInfo.Load(fp);
	if(!ret) { fclose(fp); return false; }

	ret = mSaveData.equipInfo.Load(fp);
	if(!ret) { fclose(fp); return false; }

	fclose(fp);
	return true;
}