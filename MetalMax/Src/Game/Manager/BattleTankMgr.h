#ifndef _BattleTankMgr_H_
#define _BattleTankMgr_H_

#include "../../Engine/Misc/Singleton.h"
#include <strsafe.h>

enum BattleTankID
{
	TANK_FIRST = 0,
	TANK_MAX
};

#define TANK_NAME_MAX 8
class BattleTank
{
public:
	BattleTank(BattleTankID id);
	~BattleTank(void);

	void SetName(char name[]) { strcpy_s(mName, name); mName[4] = '\0';}
	char* GetName(void) { return mName; }

	void SetSP(short sp) { mSP = sp; }
	void SetSPMax(short spmax) { mSPMax = spmax; }
	void SetAT(short at) { mAT = at; }
	void SetDF(short df) { mDF = df; }
	void SetST(short st) { mST = st; }
	void SetSTMax(short stmax) { mSTMax = stmax; }

	short GetSP(void) { return mSP; }
	short GetSPMax(void) { return mSPMax; }
	short GetAT(void) { return mAT; }
	short GetDF(void) { return mDF; }
	short GetST(void) { return mST; }
	short GetSTMax(void) { return mSTMax; }

private:
	BattleTankID mTankId;
	char mName[TANK_NAME_MAX];
	short mSP;
	short mSPMax;
	short mAT;
	short mDF;
	short mST;
	short mSTMax;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class BattleTankMgr: public Singleton<BattleTankMgr>
{
public:
	BattleTankMgr(void);
	~BattleTankMgr(void);

	BattleTank* GetTank(BattleTankID id) { return mpTanks[id]; }
	
private:
	BattleTank* mpTanks[TANK_MAX];
};
#define gBattleTankMgr BattleTankMgr::GetInstance()

#endif