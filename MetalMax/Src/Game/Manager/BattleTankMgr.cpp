#include "BattleTankMgr.h"

BattleTank::BattleTank(BattleTankID id)
:	mTankId(id),
	mSP(0),
	mSPMax(0),
	mAT(0),
	mDF(0)
{
	for(int i=0; i<TANK_NAME_MAX; i++)
	{
		mName[i] = '\0';
	}

	SetSPMax(190);
	SetAT(70);
	SetDF(50);
	SetST(32);
	SetSTMax(32);
}
	
BattleTank::~BattleTank(void)
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_SINGLETON(BattleTankMgr);
static BattleTankMgr sBattleTankMgr;

BattleTankMgr::BattleTankMgr(void)
{
	for(int i=0; i<TANK_MAX; i++)
	{
		mpTanks[i] = NULL;
	}
	mpTanks[TANK_FIRST] = new BattleTank(TANK_FIRST);
}

BattleTankMgr::~BattleTankMgr(void)
{
	for(int i=0; i<TANK_MAX; i++)
	{
		if(mpTanks[i])
		{
			delete mpTanks[i];
		}
	}
}
