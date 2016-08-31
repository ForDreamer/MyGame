#ifndef _GameStatusMgr_H_
#define _GameStatusMgr_H_

#include "../../Engine/Misc/Singleton.h"

// To Do: use state machine
enum GameState
{
	GS_START = 0,
	GS_WIN_BOSS = 1,
	GS_FIND_TANK = 2,
	GS_END_GAME = 3,
};

#define ITEM_UNLOCK_MAX 8

class GameStatusMgr: public Singleton<GameStatusMgr>
{
public:
	GameStatusMgr(void);
	~GameStatusMgr(void);

	void Reset(void);
	
	void SetGameState(GameState state) { mState = state; }
	GameState GetGameState(void) { return mState; }
	void SetGoldNum(int gold) { mGoldNum = gold; if(mGoldNum > 9999) mGoldNum = 9999;}
	int GetGoldNum(void) { return mGoldNum; }
	void SetItemUnlocked(int itemId) { assert(itemId < ITEM_UNLOCK_MAX); mItemsUnlocked[itemId] = true; }
	bool IsItemUnlocked(int itemId) { assert(itemId < ITEM_UNLOCK_MAX); return mItemsUnlocked[itemId]; }

private:
	GameState mState;
	int mGoldNum;
	bool mItemsUnlocked[ITEM_UNLOCK_MAX];
};
#define gGameStatusMgr GameStatusMgr::GetInstance()

#endif