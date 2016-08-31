#ifndef _BattleManager_H_
#define _BattleManager_H_

#include "../../Engine/Misc/Singleton.h"
#include "BattleActor.h"
#include "BattleConfig.h"
#include "BattleUIMgr.h"
#include "../Manager/ItemMgr.h"
#include <vector>
#include <algorithm>

using namespace std;

class BatEnemyList
{
public:
	BatEnemyList(void) { mBatEnemys.clear(); }
	~BatEnemyList(void) {}

	void Init(int& winExp, int& winGold);
	void Release(void);
	void Update(void);
	void Render(void);

	BattleActor* GetEnemy(int index) { return mBatEnemys[index]; }
	int GetEnemyCount(void) { return (int)mBatEnemys.size(); }
	void Insert(BattleActor* pEnemy)
	{
		assert(pEnemy);
		mBatEnemys.push_back(pEnemy);
	}
	void Remove(BattleActor* pEnemy)
	{
		assert(pEnemy);
		vector<BattleActor*>::iterator iter = find(mBatEnemys.begin(), mBatEnemys.end(), pEnemy);
		if (iter != mBatEnemys.end())
		{
			delete *iter;
			mBatEnemys.erase(iter);
		}
	}

	BattleActor* GetEnemyByEnemyId(int enemyId)
	{
		BattleActor* pEnemy = NULL;
		for(unsigned int i=0; i<mBatEnemys.size(); i++)
		{
			if(mBatEnemys[i]->GetEnemyId() == enemyId)
			{
				pEnemy = mBatEnemys[i];
				break;
			}
		}
		return pEnemy;
	}

private:
	vector<BattleActor*> mBatEnemys;
};

/////////////////////////////////////////////////////////////////////////////////////////
enum BattleMode
{
	BM_HUMAN = 0,
	BM_TANK = 1
};

enum BattleState
{
	BS_ENTER = 0,
	BS_SELECT = 1,
	BS_ATTACK = 2,
	BS_WINNING = 3,
	BS_FAIL = 4
};

enum BattleAction
{
	BA_NONE = 0,
	BA_ATTACK = 1,
	BA_GUARD = 2,
	BA_TOOL = 3,
	BA_ESCAPE = 4
};

enum AttackMode
{
	AM_NONE = 0,
	AM_TIMING_1 = 1,
	AM_TIMING_2 = 2,
	AM_SHOT_HIT = 3
};

class BattleManager: public Singleton<BattleManager>
{
	static const int MAX_EFFECT_NUM = 4;
public:
	BattleManager(void);
	~BattleManager(void);

	void Init(void);
	void Release(void);
	void Update(void);
	void Render(void);
	
	BattleMode GetBattleMode(void) { return mBattleMode; }
	void SetBattleMode(BattleMode mode) { mBattleMode = mode; mBatPlayer->RefreshTexIdByMode(); }
	bool IsActive(void) { return mbActive; }
	bool IsExitBattle(void) { return mbExitBattle; }
	bool IsEnemyDieing(void) { return mbEnemyDieing || mbDelayDie; }
	bool IsDelayingInput(void) { return mbDelayInput; }
	bool IsLevelUp(void) { return mbLevelUp; }
	bool IsBossFight(void) { return mbBossFight; }
	bool IsAttacking(void) { return mbAttacking; }
	void SetDelayInput(float delay) { mbDelayInput = true; mDelayTime = delay; mDelayTimeTicker = 0.0f; }
	void SetIsBossFight(bool boss) { mbBossFight = boss; }
	bool CheckLevelUp(void);

	int GetCurMapIdEnemyConfigNum(int configId);
	EnemyConfig* GetCurMapIdEnemyConfig(int configId, int enemyId);
	int GetEnemyNum(void) { return mBatEnemyList.GetEnemyCount(); }
	BatEnemyList& GetEnemyList(void) { return mBatEnemyList; }

	BattleActor* GetBatPlayer(void) { return mBatPlayer; }
	UIBase* GetUIMain(void)
	{
		if(mBattleMode == BM_HUMAN)
			return gBattleUIMgr.GetUI(BATUI_HUMAN_MAIN);
		else
			return gBattleUIMgr.GetUI(BATUI_TANK_MAIN);
	}

	void SetState(BattleState state) { mState = state; }
	BattleState GetState(void) { return mState; }
	void SetAction(BattleAction action) { mAction = action; }
	BattleAction GetAction(void) { return mAction; }

	void SetUseToolID(ItemID id) { mUseToolId = id; }
	void SetAttackID(int id) { mAttackId = id; }
	void ResumeAttack(void) { mbAttackStop = false; if(mBattleMode == BM_HUMAN) mBatPlayer->ResetSprTexture(); }
	bool IsLastAttacker(void) { return (mAttackCurActorId >= (int)mAttackActors.size()); }
	void StartAttackRound(void);
	void FinishAttackRound(void);
	void FinishBattle(void);
	void StartLevelUp(void);

private:
	//To Do: use state method
	void UpdateBattleStates(void);
	void UpdateEnterState(void);
	void UpdateSelectState(void);
	void UpdateAttackState(void);
	void UpdateWinningState(void);
	void UpdateFailState(void);
	
	void DoAttack(BattleActor* pAttacker, BattleActor* pDefender);
	void DoGuarding(void);
	void DoUseTool(void);
	void DoEscape(void);
	void ProcessAttacking(void);

	void StartEnemyDieing(int enmeyId);
	void UpdateEnemyDieing(void);

	void SetupAttackAnimEffect(int index, float posX, float posY, float sizeFactor, int texId, int animMode, float scaleX=1.0f);
	void ClearAttackAnimEffects(void);
	void SetupAttackShotEffect(float posX, float posY, float sizeFactor, int texId, float targetX, float targetY, float speed);
	void ClearAttackShotEffect(void);

private:
	BattleMode mBattleMode;
	BattleState mState;
	BattleAction mAction;
	AttackMode mAttackMode;
	bool mbActive;
	bool mbExitBattle;
	bool mbDelayInput;
	bool mbLevelUp;
	bool mbBossFight;
	bool mbAttacking;
	float mDelayTime;
	float mDelayTimeTicker;

	BattleActor* mBatPlayer;
	BatEnemyList mBatEnemyList;
	int mWinExp;
	int mWinGold;

	float mEnterStateTimer;
	int mEnterStateMaxStep;
	int mEnterStateCurStep;

	float mAttackStateTimer;
	bool mbAttackStop;
	int mAttackId;
	int mAttackCurActorId;
	ItemID mUseToolId;
	vector<BattleActor*> mAttackActors;
	BattleActor* mpCurAttacker;
	BattleActor* mpCurDefender;
	float mAttackingTimer;
	WeaponType mCurWeapon;
	bool mbDelayDie;
	float mbDelayDieTimer;
	BattleActor* mpCurActorDie;

	bool mbEnemyDieing;
	float mEnemyDieingTime;
	int mbEnemyDieId;

	Sprite* mpSprEffects[MAX_EFFECT_NUM];
	Sprite* mpSprShot;
	bool mbAnimShot;
	float mShotDeltaX;
	float mShotDeltaY;
	float mShotInitDX;
	float mShotInitDY;
	float mShotSpeed;
};
#define gBattleMgr BattleManager::GetInstance()

#endif