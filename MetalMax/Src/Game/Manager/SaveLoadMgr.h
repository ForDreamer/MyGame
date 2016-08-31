#ifndef _SaveLoadMgr_H_
#define _SaveLoadMgr_H_

#include "../../Engine/Misc/Singleton.h"
#include <stdio.h>

static const char sSaveFilename[32] = "../Save/SaveData.sav";

struct SLMapInfo
{
	short playerMapId;
	short playerTileX;
	short playerTileY;
	short playerDir;
	short tankMapId;
	short tankTileX;
	short tankTileY;
	short tankDir;

	void Reset(void)
	{
		playerMapId = 0;
		playerTileX = 23;
		playerTileY = 17;
		playerDir = 0;
		tankMapId = 14;
		tankTileX = 31;
		tankTileY = 32;
		tankDir = 3;
	}
	bool Save(FILE* fp);
	bool Load(FILE* fp);
	void SaveFromGame(void);
	void LoadToGame(void);
};

struct SLCharInfo
{
	char name[8];
	short HP;
	short HPMax;
	short AT;
	short DF;
	short STR;
	short INT;
	short AGL;
	short PHY;
	short LV;
	short EXP;

	void Reset(void)
	{
		name[0]='N'; name[1]='x'; name[2]='A'; name[3]='F';
		name[4]='\0'; name[5]='\0'; name[6]='\0'; name[7]='\0';
		HP = 80;
		HPMax = 80;
		AT = 15;
		DF = 23;
		STR = 10;
		INT = 10;
		AGL = 20;
		PHY = 15;
		LV = 1;
		EXP = 0;
	}
	bool Save(FILE* fp);
	bool Load(FILE* fp);
	void SaveFromGame(void);
	void LoadToGame(void);
};

struct SLTankInfo
{
	char name[8];
	short SP;
	short ST;
	
	void Reset(void)
	{
		name[0]='T'; name[1]='a'; name[2]='n'; name[3]='k';
		name[4]='\0'; name[5]='\0'; name[6]='\0'; name[7]='\0';
		SP = 190;
		ST = 32;
	}
	bool Save(FILE* fp);
	bool Load(FILE* fp);
	void SaveFromGame(void);
	void LoadToGame(void);
};

struct SLStatusInfo
{
	short state;
	short gold;
	bool searchUnlocks[8];

	void Reset(void)
	{
		state = 0;
		gold = 50;
		for(int i=0; i<8; i++)
		{
			searchUnlocks[i] = false;
		}
	}
	bool Save(FILE* fp);
	bool Load(FILE* fp);
	void SaveFromGame(void);
	void LoadToGame(void);
};

struct SLItemInfo
{
	short itemIDs[8];

	void Reset(void)
	{
		itemIDs[0] = 0;
		itemIDs[1] = 1;
		itemIDs[2] = 2;
		itemIDs[3] = 3;
		itemIDs[4] = 4;
		itemIDs[5] = -1;
		itemIDs[6] = -1;
		itemIDs[7] = -1;
	}
	bool Save(FILE* fp);
	bool Load(FILE* fp);
	void SaveFromGame(void);
	void LoadToGame(void);
};

struct SLEquipInfo
{
	short equipIDs[8];
	bool equiped[8];

	void Reset(void)
	{
		equipIDs[0] = 0;
		equipIDs[1] = 2;
		equiped[0] = true;
		equiped[1] = true;
		for(int i=2; i<8; i++)
		{
			equipIDs[i] = -1;
			equiped[i] = false;
		}
	}
	bool Save(FILE* fp);
	bool Load(FILE* fp);
	void SaveFromGame(void);
	void LoadToGame(void);
};

struct SaveData
{
	SLMapInfo mapInfo;
	SLCharInfo charInfo;
	SLTankInfo tankInfo;
	SLStatusInfo statusInfo;
	SLItemInfo itemInfo;
	SLEquipInfo equipInfo;

	// To Do: refactor use oop
	void Reset(void)
	{
		mapInfo.Reset();
		charInfo.Reset();
		tankInfo.Reset();
		statusInfo.Reset();
		itemInfo.Reset();
		equipInfo.Reset();
	}
	void SaveFromGame(void)
	{
		mapInfo.SaveFromGame();
		charInfo.SaveFromGame();
		tankInfo.SaveFromGame();
		statusInfo.SaveFromGame();
		itemInfo.SaveFromGame();
		equipInfo.SaveFromGame();
	}
	void LoadToGame(void)
	{
		mapInfo.LoadToGame();
		charInfo.LoadToGame();
		tankInfo.LoadToGame();
		statusInfo.LoadToGame();
		itemInfo.LoadToGame();
		equipInfo.LoadToGame();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class SaveLoadMgr: public Singleton<SaveLoadMgr>
{
public:
	SaveLoadMgr(void);
	~SaveLoadMgr(void);
	
	void Initialize(void);
	bool Save(void);
	bool Load(void);
	void ResetSaveData(void) { mSaveData.Reset(); }
	void SaveSaveData_FromGame(void) { mSaveData.SaveFromGame(); }
	void LoadSaveData_ToGame(void) { mSaveData.LoadToGame(); }

	const SLMapInfo& GetMapInfo(void) { return mSaveData.mapInfo; }
	const SLCharInfo& GetCharInfo(void) { return mSaveData.charInfo; }
	const SLTankInfo& GetTankInfo(void) { return mSaveData.tankInfo; }
	const SLStatusInfo& GetStatusInfo(void) { return mSaveData.statusInfo; }
	const SLItemInfo& GetItemInfo(void) { return mSaveData.itemInfo; }
	const SLEquipInfo& GetEquipInfo(void) { return mSaveData.equipInfo; }
	
private:
	bool SaveSingleFile(const char* filename);
	bool LoadSingleFile(const char* filename);

private:
	SaveData mSaveData;
};

#define gSaveLoadMgr SaveLoadMgr::GetInstance()

#endif // _SAVELOAD_MANAGER_H
