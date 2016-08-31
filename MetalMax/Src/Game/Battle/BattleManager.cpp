#include "BattleManager.h"
#include "BatUITextBox.h"
#include "BatUIInfoBox.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../MapEngine/MapManager.h"
#include "../Manager/GameStatusMgr.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/BattleTankMgr.h"

static const int sLevelUpTable[10] = {0,10,30,60,100, 160,220,320,500,1000};
static const int sHpMaxTable[10] = {0,80,95,110,125, 150,175,200,230,260};

void BatEnemyList::Init(int& winExp, int& winGold)
{
	if(gBattleMgr.IsBossFight())
	{
		BattleActor* pBatBoss = new BattleActor();
		pBatBoss->Init(TEX_BAT_BOSS, 0, -220.0f, 80.0f, 144.0f, 120.0f);
		pBatBoss->SetupBatAttribs(TEX_BAT_BOSS, '\0');
		pBatBoss->SetVisible(false);
		Insert(pBatBoss);

		winExp += pBatBoss->GetExp();
		winGold += pBatBoss->GetGold();
	}
	else
	{
		int configId = rand() % MAX_ENEMY_CONFIGS;
		int enemyNum = gBattleMgr.GetCurMapIdEnemyConfigNum(configId);
		for(int i=0; i<enemyNum; i++)
		{
			EnemyConfig* pConfig = gBattleMgr.GetCurMapIdEnemyConfig(configId, i);
			BattleActor* pBatActor = new BattleActor();
			pBatActor->Init(pConfig->texId, i, pConfig->posX, pConfig->posY, pConfig->sizeX, pConfig->sizeY);
			pBatActor->SetupBatAttribs(pConfig->texId, pConfig->nameFix);
			pBatActor->SetVisible(false);
			Insert(pBatActor);
			
			winExp += pBatActor->GetExp();
			winGold += pBatActor->GetGold();
		}
	}
}

void BatEnemyList::Release(void)
{
	for(unsigned int i=0; i<mBatEnemys.size(); i++)
		delete mBatEnemys[i];
	mBatEnemys.clear();
}

void BatEnemyList::Update(void)
{
	for(unsigned int i=0; i<mBatEnemys.size(); i++)
		mBatEnemys[i]->Update();
}

void BatEnemyList::Render(void)
{
	for(unsigned int i=0; i<mBatEnemys.size(); i++)
		mBatEnemys[i]->Render();
}

/////////////////////////////////////////////////////////////////////////////////////////
DECLARE_SINGLETON(BattleManager);
static BattleManager sBattleManager;

BattleManager::BattleManager(void)
:	mBattleMode(BM_HUMAN),
	mState(BS_ENTER),
	mAction(BA_NONE),
	mAttackMode(AM_NONE),
	mbActive(false),
	mbExitBattle(false),
	mbDelayInput(false),
	mbLevelUp(false),
	mbBossFight(false),
	mbAttacking(false),

	mDelayTime(0.0f),
	mDelayTimeTicker(0.0f),
	mWinExp(0),
	mWinGold(0),
	mBatPlayer(NULL),
	mpCurAttacker(NULL),
	mpCurDefender(NULL),
	mAttackingTimer(0.0f),
	mCurWeapon(WT_NONE),
	mbDelayDie(false),
	mbDelayDieTimer(0.0f),
	mpCurActorDie(NULL),

	mEnterStateTimer(0.0f),
	mEnterStateMaxStep(0),
	mEnterStateCurStep(0),
	mAttackStateTimer(0.0f),
	mbAttackStop(false),
	mAttackId(0),
	mAttackCurActorId(0),

	mbAnimShot(false),
	mShotDeltaX(0.0f),
	mShotDeltaY(0.0f),
	mShotInitDX(0.0f),
	mShotInitDY(0.0f),
	mShotSpeed(0.0f)
{
}
	
BattleManager::~BattleManager(void)
{
}

void BattleManager::Release(void)
{
	mbActive = false;
	mbBossFight = false;

	if(mBatPlayer)
	{
		delete mBatPlayer;
		mBatPlayer = NULL;
	}
	mBatEnemyList.Release();
	mAttackActors.clear();

	for(int i=0; i<MAX_EFFECT_NUM; i++)
	{
		delete mpSprEffects[i];
		mpSprEffects[i] = NULL;
	}
	delete mpSprShot;
	mpSprShot = NULL;
}

void BattleManager::Init(void)
{
	int playerTexId;
	if(!gMapMgr.IsPlayerInTank())
	{
		mBattleMode = BM_HUMAN;
		playerTexId = TEX_BAT_PLAYER;
	}
	else
	{
		mBattleMode = BM_TANK;
		playerTexId = TEX_BAT_TANK;
	}
	mbActive = true;
	mbExitBattle = false;
	mbLevelUp = false;
	mWinExp = 0;
	mWinGold = 0;

	mBatPlayer = new BattleActor();
	mBatPlayer->Init(playerTexId, -1, 240.0f, 90.0f, 72.0f, 60.0f);
	mBatPlayer->SetupBatAttribs(-1, '\0');
	mBatEnemyList.Init(mWinExp, mWinGold);

	UIBase* pUIMain = GetUIMain();
	pUIMain->OpenUI();
	pUIMain->SetOnFocus(false);
	UIBase* pUITextBox = gBattleUIMgr.GetUI(BATUI_TEXTBOX);
	pUITextBox->OpenUI();
	pUITextBox->SetOnFocus(false);

	SetAction(BA_NONE);
	SetState(BS_ENTER);
	mEnterStateMaxStep = GetEnemyNum();
	mEnterStateCurStep = 0;

	mAttackStateTimer = 0.0f;
	mAttackId = -1;
	mAttackCurActorId = 0;
	mbAttackStop = false;
	mbAttacking = false;
	mUseToolId = ITEM_INVALID;
	mAttackActors.clear();
	mAttackMode = AM_NONE;
	mpCurAttacker = NULL;
	mpCurDefender = NULL;
	mAttackingTimer = 0.0f;
	mCurWeapon = WT_NONE;
	mbDelayDie = false;
	mbDelayDieTimer = 0.0f;
	mpCurActorDie = NULL;

	mbEnemyDieing = false;
	mEnemyDieingTime = 0.0f;
	mbEnemyDieId = -1;

	
	for(int i=0; i<MAX_EFFECT_NUM; i++)
	{
		mpSprEffects[i] = new Sprite();
		mpSprEffects[i]->SetVisible(false);
		mpSprEffects[i]->SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	}
	mpSprShot = new Sprite();
	mpSprShot->SetVisible(false);
	mpSprShot->SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mbAnimShot = false;
	mShotDeltaX = 0.0f;
	mShotDeltaY = 0.0f;
	mShotInitDX = 0.0f;
	mShotInitDY = 0.0f;
	mShotSpeed = 0.0f;
}

void BattleManager::Update(void)
{
	if(mbDelayInput)
	{
		mDelayTimeTicker += Global::GetTimeDelta();
		if(mDelayTimeTicker >= mDelayTime)
		{
			mDelayTime = 0.0f;
			mDelayTimeTicker = 0.0f;
			mbDelayInput = false;
		}
	}
	UpdateBattleStates();
	
	mBatPlayer->Update();
	mBatEnemyList.Update();

	for(int i=0; i<MAX_EFFECT_NUM; i++)
	{
		if(mpSprEffects[i]->GetVisible())
			mpSprEffects[i]->Update();
	}
	if(mbAnimShot)
	{
		float curX = mpSprShot->GetPositionX();
		float curY = mpSprShot->GetPositionY();
		mShotDeltaX = mShotInitDX * Global::GetTimeDelta() * mShotSpeed;
		mShotDeltaY = mShotInitDY * Global::GetTimeDelta() * mShotSpeed;
		curX += mShotDeltaX;
		curY += mShotDeltaY;
		mpSprShot->SetPositionX(curX);
		mpSprShot->SetPositionY(curY);
	}
	if(mpSprShot->GetVisible())
		mpSprShot->Update();
}

void BattleManager::Render(void)
{
	mBatPlayer->Render();
	mBatEnemyList.Render();

	for(int i=0; i<MAX_EFFECT_NUM; i++)
	{
		if(mpSprEffects[i]->GetVisible())
			mpSprEffects[i]->Render();
	}
	if(mpSprShot->GetVisible())
		mpSprShot->Render();
}


int BattleManager::GetCurMapIdEnemyConfigNum(int configId)
{
	assert(configId < MAX_ENEMY_CONFIGS);
	MapID mapId = gMapMgr.GetCurMapID();
	int enemyNum = 0;

	switch(mapId)
	{
	case MAPID_WORLD: enemyNum = sEnemyNum_World[configId]; break;
	case MAPID_CAVE_1: enemyNum = sEnemyNum_Cave2[configId]; break;
	case MAPID_CAVE_2: enemyNum = sEnemyNum_Cave2[configId]; break;
	case MAPID_CAVE_3: enemyNum = sEnemyNum_Cave3[configId]; break;
	}
	assert(enemyNum <= MAX_ENEMY_NUM);

	return enemyNum;
}

EnemyConfig* BattleManager::GetCurMapIdEnemyConfig(int configId, int enemyId)
{
	assert(configId < MAX_ENEMY_CONFIGS);
	assert(enemyId < MAX_ENEMY_NUM);
	MapID mapId = gMapMgr.GetCurMapID();
	EnemyConfig* pConfig = NULL;

	switch(mapId)
	{
	case MAPID_WORLD: pConfig = &sEnemyConfigs_World[configId][enemyId]; break;
	case MAPID_CAVE_1: pConfig = &sEnemyConfigs_Cave2[configId][enemyId]; break;
	case MAPID_CAVE_2: pConfig = &sEnemyConfigs_Cave2[configId][enemyId]; break;
	case MAPID_CAVE_3: pConfig = &sEnemyConfigs_Cave3[configId][enemyId]; break;
	}
	assert(pConfig);

	return pConfig;
}


void BattleManager::UpdateBattleStates(void)
{
	switch(mState)
	{
		case BS_ENTER: UpdateEnterState(); break;
		case BS_SELECT: UpdateSelectState(); break;
		case BS_ATTACK: UpdateAttackState(); break;
		case BS_WINNING: UpdateWinningState(); break;
		case BS_FAIL: UpdateFailState(); break;
	}
}

void BattleManager::UpdateEnterState(void)
{
	UIBase* pUIMain = GetUIMain();
	BatUITextBox* pUITextBox = (BatUITextBox*)gBattleUIMgr.GetUI(BATUI_TEXTBOX);

	if(mEnterStateCurStep < mEnterStateMaxStep)
	{
		float WAITING_TIME = 0.3f;
		if(mEnterStateCurStep == 0)
			WAITING_TIME = 0.7f;

		mEnterStateTimer += Global::GetTimeDelta();
		if(mEnterStateTimer > WAITING_TIME)
		{
			BattleActor* pEnemy = mBatEnemyList.GetEnemy(mEnterStateCurStep);
			pEnemy->SetVisible(true);
			FmodSound::PlaySnd(SND_ENEMYAPPEAR);
			
			char tempstr[32];
			sprintf(tempstr, "%s出现了！", pEnemy->GetName());
			pUITextBox->SetLineText(mEnterStateCurStep, tempstr);
			mEnterStateTimer = 0;
			mEnterStateCurStep++;
		}
	}
	else
	{
		mEnterStateTimer += Global::GetTimeDelta();
		if(mEnterStateTimer > 1.0f)
		{
			mEnterStateTimer = 0;
			SetState(BS_SELECT);
			
			pUITextBox->CloseUI();
			pUIMain->SetOnFocus(true);
		}
	}
}

void BattleManager::UpdateSelectState(void)
{
}

// To Do: refactor attack logic
void BattleManager::UpdateAttackState(void)
{
	if(mbDelayDie)
	{
		mbDelayDieTimer += Global::GetTimeDelta();
		if(mbDelayDieTimer > 0.5f)
		{
			BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
			char tempstr[64];
			sprintf(tempstr, "%s 被打倒了！", mpCurActorDie->GetName());
			pInfoBox->SetLineText(2, tempstr);
			if(mpCurActorDie->IsPlayer())
			{
				if(mBattleMode == BM_HUMAN)
					mpCurActorDie->SetSprTexture(TEX_BAT_DIE);
				SetState(BS_FAIL);
				mbExitBattle = true;
				FmodSound::StopMusic();
				FmodSound::PlaySnd(SND_FAIL);
				SetDelayInput(11.0f);
			}
			else
			{
				StartEnemyDieing(mpCurActorDie->GetEnemyId());
			}

			mbDelayDie = false;
			mbDelayDieTimer = 0.0f;
			mpCurActorDie = NULL;
		}
		return;
	}

	if(mbEnemyDieing)
	{
		UpdateEnemyDieing();
		return;
	}

	if(!mbAttackStop)
	{
		float WAITING_TIME = 0.25f;
		if(mAction != BA_ATTACK || mAttackCurActorId == 0)
			WAITING_TIME = 0.5f;

		mAttackStateTimer += Global::GetTimeDelta();
		if(mAttackStateTimer > WAITING_TIME)
		{
			if(mAction == BA_ATTACK)
			{
				BattleActor* pAttacker = mAttackActors[mAttackCurActorId];
				BattleActor* pDefender;
				if(pAttacker->IsPlayer())
				{
					assert(mAttackId != -1 && mAttackId < GetEnemyNum());
					pDefender = mBatEnemyList.GetEnemy(mAttackId);
				}
				else
					pDefender = mBatPlayer;

				assert(pAttacker && pDefender);
				DoAttack(pAttacker, pDefender);
				mAttackCurActorId++;
			}
			else
			{
				switch(mAction)
				{
				case BA_GUARD: DoGuarding(); break;
				case BA_TOOL: DoUseTool(); break;
				case BA_ESCAPE: DoEscape(); break;
				}
				SetAction(BA_ATTACK);
			}

			mAttackStateTimer = 0;
			mbAttackStop = true;
		}
	}

	if(mbAttacking)
		ProcessAttacking();
}

void BattleManager::UpdateWinningState(void)
{
}

void BattleManager::UpdateFailState(void)
{
}


bool CompareBatActorSpeed(BattleActor* a, BattleActor* b)
{
	return a->GetSpeed() > b->GetSpeed();
}

void BattleManager::StartAttackRound(void)
{
	SetState(BS_ATTACK);

	if(mAction == BA_ATTACK)
		mAttackActors.push_back(mBatPlayer);

	int enemyNum = GetEnemyNum();
	for(int i=0; i<enemyNum; i++)
	{
		BattleActor* pEnemy = mBatEnemyList.GetEnemy(i);
		mAttackActors.push_back(pEnemy);
	}
	sort(mAttackActors.begin(), mAttackActors.end(), CompareBatActorSpeed);

	mAttackCurActorId = 0;
	mAttackStateTimer = 0.0f;
	mbAttackStop = false;
}

void BattleManager::FinishAttackRound(void)
{
	mAttackActors.clear();
	mBatPlayer->SetGuarding(false);
	if(mBattleMode == BM_HUMAN)
		mBatPlayer->ResetSprTexture();
	mAttackId = -1;
	mUseToolId = ITEM_INVALID;

	SetState(BS_SELECT);
	SetAction(BA_NONE);

	if(mBatEnemyList.GetEnemyCount() == 0)
		FinishBattle();
}

void BattleManager::FinishBattle(void)
{
	SetState(BS_WINNING);
	FmodSound::PlaySnd(SND_WINNING);
	FmodSound::StopMusic();
	if(mBattleMode == BM_HUMAN)
		mBatPlayer->ResetSprTexture();

	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	pInfoBox->OpenUI();
	pInfoBox->SetLineText(0, "教训了敌人！");
	char tempstr[32];
	sprintf(tempstr, "获得%d分经验值！", mWinExp);
	pInfoBox->SetLineText(1, tempstr);
	sprintf(tempstr, "%dG到手了！", mWinGold);
	pInfoBox->SetLineText(2, tempstr);

	UIBase* pUIMain = GetUIMain();
	pUIMain->SetOnFocus(false);
	mbExitBattle = true;
	SetDelayInput(1.5f);
	
	BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
	pTank->SetSP(mBatPlayer->GetSP());
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	pPlayer->SetAttrib(CAB_HP, mBatPlayer->GetHP());
	int curExp = pPlayer->GetAttrib(CAB_EXP);
	pPlayer->SetAttrib(CAB_EXP, curExp+mWinExp);

	if(CheckLevelUp())
	{
		mbLevelUp = true;
		mbExitBattle = false;
		SetDelayInput(2.0f);
	}

	gGameStatusMgr.SetGoldNum(gGameStatusMgr.GetGoldNum() + mWinGold);
}


void BattleManager::StartEnemyDieing(int enmeyId)
{
	mbEnemyDieing = true;
	mEnemyDieingTime = 0.0f;
	mbEnemyDieId = enmeyId;

	FmodSound::PlaySnd(SND_DISAPPEAR);
	BattleActor* pEnemy = mBatEnemyList.GetEnemyByEnemyId(mbEnemyDieId);
	pEnemy->SetDieBlink(true);
}

void BattleManager::UpdateEnemyDieing(void)
{
	mEnemyDieingTime += Global::GetTimeDelta();
	if(mEnemyDieingTime > 0.5f)
	{
		BattleActor* pEnemy = mBatEnemyList.GetEnemyByEnemyId(mbEnemyDieId);

		mAttackActors.erase( remove(mAttackActors.begin(), mAttackActors.end(), pEnemy) );
		mBatEnemyList.Remove(pEnemy);
		
		mEnemyDieingTime = 0.0f;
		mbEnemyDieing = false;
	}
}



void BattleManager::ProcessAttacking(void)
{
	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	char tempstr[64];
	bool doDamage = false;

	assert(mAttackMode != AM_NONE);
	if(mAttackMode == AM_TIMING_1)
	{
		mAttackingTimer += Global::GetTimeDelta();
		if(mAttackingTimer > 0.5f)
			doDamage = true;
	}
	else if(mAttackMode == AM_TIMING_2)
	{
		mAttackingTimer += Global::GetTimeDelta();
		if(mAttackingTimer > 0.1f)
			doDamage = true;
	}
	else if(mAttackMode == AM_SHOT_HIT)
	{
		float shotX = mpSprShot->GetPositionX();
		float targetX = mpCurDefender->GetPosX();
		if( (shotX >= targetX-fabs(mShotDeltaX) && shotX <= targetX+fabs(mShotDeltaX)) || fabs(shotX) > 500.0f )
		{
			ClearAttackShotEffect();
			doDamage = true;
		}
	}

	if(doDamage)
	{
		BattleActor* pAttacker = mpCurAttacker;
		BattleActor* pDefender = mpCurDefender;

		int damage = 0;
		int AT = 0;
		int DF = 0;

		AT = pAttacker->GetAT();
		if(pAttacker->IsPlayer() && mBattleMode == BM_TANK)
			AT = 100;
		else if(pAttacker->IsPlayer() && mUseToolId == ITEM_BOMB)
			AT = 40;
		else if(pAttacker->IsPlayer() && mUseToolId == ITEM_FIRE)
			AT = 50;
		AT += rand()%7-3;

		DF = pDefender->GetDF();
		if(pDefender->IsPlayer() && mBattleMode == BM_TANK && pDefender->GetSP() > 0)
			DF = 50;

		damage = AT - DF;
		if(pDefender->IsGuarding())
			damage /= 2;
		if(damage < 0)
			damage = 0;
		if(damage > 0)
		{
			if(pDefender->IsPlayer() && mBattleMode == BM_TANK && pDefender->GetSP() > 0)
				pDefender->MinusSP(damage);
			else
				pDefender->MinusHP(damage);
		}

		if(damage > 0)
		{
			sprintf(tempstr, "%s 损伤了%d！", pDefender->GetName(), damage);
			pInfoBox->SetLineText(1, tempstr);
			if(pDefender->GetHP() == 0)
			{
				mbDelayDie = true;
				mbDelayDieTimer = 0.0f;
				mpCurActorDie = pDefender;
			}
		}
		else
		{
			sprintf(tempstr, "%s 没有受到损伤！", pDefender->GetName());
			pInfoBox->SetLineText(1, tempstr);
		}

		switch(mCurWeapon)
		{
			case WT_BODY: FmodSound::PlaySnd(SND_WT_BODY_END); break;
			case WT_SLING:
				FmodSound::PlaySnd(SND_WT_SLING_END);
				SetupAttackAnimEffect(0, pDefender->GetPosX(), pDefender->GetPosY(), 1.5f, TEX_BAT_EFFECT_GUN_1, 2);
				break;
			case WT_CANNON:
				FmodSound::PlaySnd(SND_WT_CANNON_END);
				SetupAttackAnimEffect(0, pDefender->GetPosX(), pDefender->GetPosY(), 4.0f, TEX_BAT_EFFECT_FIRE, 1);
				break;
			case WT_FIRE: 
				FmodSound::PlaySnd(SND_WT_CANNON_END);
				SetupAttackAnimEffect(0, pDefender->GetPosX(), pDefender->GetPosY(), 4.0f, TEX_BAT_EFFECT_FIRE, 1);
				break;
			case WT_BOMB:
				FmodSound::PlaySnd(SND_WT_CANNON_END);
				SetupAttackAnimEffect(0, pDefender->GetPosX(), pDefender->GetPosY(), 4.0f, TEX_BAT_EFFECT_BOMB, 1);
				break;
		}
		pAttacker->SetAttackBlink(false);
		if(pDefender->GetHP() > 0)
			pDefender->SetBeenDamaged(true);

		mAttackingTimer = 0.0f;
		mpCurAttacker = NULL;
		mpCurDefender = NULL;
		mAttackMode = AM_NONE;
		mCurWeapon = WT_NONE;
		mbAttacking = false;

		if(!mbDelayDie)
			SetDelayInput(0.5f);
	}
}

void BattleManager::DoAttack(BattleActor* pAttacker, BattleActor* pDefender)
{
	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	char tempstr[64];
	sprintf(tempstr, "%s 的攻击！", pAttacker->GetName());
	pInfoBox->SetLineText(0, tempstr);
	
	AttackMode attackMode;
	WeaponType attackWeapon;
	if(pAttacker->IsPlayer())
	{
		if(mUseToolId == ITEM_BOMB)
			attackWeapon = WT_BOMB;
		else if(mUseToolId == ITEM_FIRE)
			attackWeapon = WT_FIRE;
		else if(mBattleMode == BM_TANK)
			attackWeapon = WT_CANNON;
		else
			attackWeapon = pAttacker->GetWeaponTypeByEquip();

		if(attackWeapon == WT_GUN)
		{
			pAttacker->SetSprTexture(TEX_BAT_PLAYER_GUN);
			SetupAttackAnimEffect(0, pDefender->GetPosX(), pDefender->GetPosY(), 1.0f, TEX_BAT_EFFECT_GUN_1, 0);
			SetupAttackAnimEffect(1, pAttacker->GetPosX()-30.0f, pAttacker->GetPosY(), 1.0f, TEX_BAT_EFFECT_GUN_2, 1);
		}
		else if(attackWeapon == WT_SLING)
		{
			pAttacker->SetSprTexture(TEX_BAT_PLAYER_SLING);
			SetupAttackShotEffect(pAttacker->GetPosX()-30.0f, pAttacker->GetPosY(), 0.25f, TEX_BAT_SLING_SHOT, 
				pDefender->GetPosX(), pDefender->GetPosY(), 750.0f);
		}
		else if(attackWeapon == WT_BOMB)
		{
			pAttacker->SetSprTexture(TEX_BAT_PLAYER_TOOL);
			SetupAttackShotEffect(pAttacker->GetPosX()-35.0f, pAttacker->GetPosY(), 1.0f, TEX_BAT_BOMB_SHOT, 
				pDefender->GetPosX(), pDefender->GetPosY(), 750.0f);
		}
		else if(attackWeapon == WT_FIRE)
		{
			pAttacker->SetSprTexture(TEX_BAT_PLAYER_TOOL);
			SetupAttackShotEffect(pAttacker->GetPosX()-35.0f, pAttacker->GetPosY(), 1.0f, TEX_BAT_FIRE_SHOT, 
				pDefender->GetPosX(), pDefender->GetPosY(), 750.0f);
		}
		else if(attackWeapon == WT_CANNON)
		{
			SetupAttackAnimEffect(1, pAttacker->GetPosX()-35.0f, pAttacker->GetPosY()+5.0f, 2.0f, TEX_BAT_EFFECT_CANNON, 3);
			SetupAttackShotEffect(pAttacker->GetPosX()-35.0f, pAttacker->GetPosY()+5.0f, 1.0f, TEX_BAT_CANNON_SHOT, 
				pDefender->GetPosX(), pDefender->GetPosY(), 1000.0f);
		}
		else if(attackWeapon == WT_BODY)
		{
			pAttacker->SetAttackBlink(true);
		}
	}
	else
	{
		attackWeapon = pAttacker->GetWeaponType();
		float xOffset = 0.0f;
		float yOffset = 0.0f;
		if(attackWeapon == WT_GUN)
		{
			pAttacker->SetAttackBlink(true);
			SetupAttackAnimEffect(0, pDefender->GetPosX(), pDefender->GetPosY(), 1.0f, TEX_BAT_EFFECT_GUN_1, 0);
			if(pAttacker->GetTexId() == TEX_BAT_SNAIL)
				xOffset = 35.0f;
			else if(pAttacker->GetTexId() == TEX_BAT_GUN)
				xOffset = 68.0f;
			SetupAttackAnimEffect(1, pAttacker->GetPosX()+xOffset, pAttacker->GetPosY()+5.0f, 1.0f, TEX_BAT_EFFECT_GUN_2, 1, -1.0f);
		}
		else if(attackWeapon == WT_FIRE)
		{
			pAttacker->SetAttackBlink(true);
			SetupAttackShotEffect(pAttacker->GetPosX()+35.0f, pAttacker->GetPosY()+20.0f, 1.75f, TEX_BAT_FIRE_SHOT_2, 
				pDefender->GetPosX(), pDefender->GetPosY(), 750.0f);
		}
		else if(attackWeapon == WT_CANNON)
		{
			if(pAttacker->GetTexId() == TEX_BAT_CANNON)
			{
				xOffset = 60.0f;
				yOffset = 8.0f;
			}
			else if(pAttacker->GetTexId() == TEX_BAT_BOSS)
			{
				xOffset = 60.0f;
				yOffset = 25.0f;
			}
			SetupAttackAnimEffect(1, pAttacker->GetPosX()+xOffset, pAttacker->GetPosY()+yOffset, 2.0f, TEX_BAT_EFFECT_CANNON, 3, -1.0f);
			SetupAttackShotEffect(pAttacker->GetPosX()+xOffset, pAttacker->GetPosY()+yOffset, 1.0f, TEX_BAT_CANNON_SHOT_2, 
				pDefender->GetPosX(), pDefender->GetPosY(), 1000.0f);
		}
		else
		{
			pAttacker->SetAttackBlink(true);
		}
	}

	switch(attackWeapon)
	{
		case WT_BODY: attackMode = AM_TIMING_1; FmodSound::PlaySnd(SND_WT_BODY); break;
		case WT_SLING: attackMode = AM_SHOT_HIT; FmodSound::PlaySnd(SND_WT_SLING); break;
		case WT_GUN: attackMode = AM_TIMING_2; FmodSound::PlaySnd(SND_WT_GUN); break;
		case WT_CANNON: attackMode = AM_SHOT_HIT; FmodSound::PlaySnd(SND_WT_CANNON); break;
		case WT_FIRE: 
			attackMode = AM_SHOT_HIT;
			if(pAttacker->IsPlayer())
				FmodSound::PlaySnd(SND_WT_SLING);
			else
				FmodSound::PlaySnd(SND_WT_FIRE);
			break;
		case WT_BOMB: attackMode = AM_SHOT_HIT; FmodSound::PlaySnd(SND_WT_SLING); break;
	}
	
	mAttackingTimer = 0.0f;
	mpCurAttacker = pAttacker;
	mpCurDefender = pDefender;
	mAttackMode = attackMode;
	mCurWeapon = attackWeapon;
	mbAttacking = true;
}

void BattleManager::DoGuarding(void)
{
	mBatPlayer->SetGuarding(true);

	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	char tempstr[32];
	sprintf(tempstr, "%s 在防御！", mBatPlayer->GetName());
	pInfoBox->SetLineText(0, tempstr);
}

void BattleManager::DoUseTool(void)
{
	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	char tempstr[64];

	// To Do: refactor item effect
	switch(mUseToolId)
	{
	case ITEM_TOWEL:
		sprintf(tempstr, "%s 使用了毛巾，", mBatPlayer->GetName());
		pInfoBox->SetLineText(0, tempstr);
		pInfoBox->SetLineText(1, "但什么也没发生。");
		break;
	case ITEM_COIN:
		sprintf(tempstr, "%s 使用了古币，", mBatPlayer->GetName());
		pInfoBox->SetLineText(0, tempstr);
		pInfoBox->SetLineText(1, "但什么也没发生。");
		break;
	case ITEM_HP:
		sprintf(tempstr, "%s 喝了参丸！", mBatPlayer->GetName());
		pInfoBox->SetLineText(0, tempstr);
		mBatPlayer->AddHP(100);
		if(mBatPlayer->IsMaxHP())
			pInfoBox->SetLineText(1, "HP全满了！");
		else
			pInfoBox->SetLineText(1, "HP恢复了100！");
		break;

	}
}

void BattleManager::DoEscape(void)
{
	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	char tempstr[64];

	int rate = rand()%100;
	if(mbBossFight)
		rate = 0;
	if(rate > 30)
	{
		sprintf(tempstr, "%s 逃跑了..", mBatPlayer->GetName());
		pInfoBox->SetLineText(0, tempstr);
		mbExitBattle = true;

		BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
		pTank->SetSP(mBatPlayer->GetSP());
		Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
		pPlayer->SetAttrib(CAB_HP, mBatPlayer->GetHP());
	}
	else
	{
		sprintf(tempstr, "%s 在逃跑..", mBatPlayer->GetName());
		pInfoBox->SetLineText(0, tempstr);
		pInfoBox->SetLineText(1, "但被怪物包围了！");
	}
	FmodSound::PlaySnd(SND_ESCAPE);
}

void BattleManager::StartLevelUp(void)
{
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	int curLv = pPlayer->GetAttrib(CAB_LV);

	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);
	pInfoBox->OpenUI();
	char tempstr[32];
	sprintf(tempstr, "%s 水平提高到Lv%d！", mBatPlayer->GetName(), curLv);
	pInfoBox->SetLineText(0, tempstr);

	FmodSound::PlaySnd(SND_LEVELUP);
	SetDelayInput(5.0f);
	mbExitBattle = true;
}

bool BattleManager::CheckLevelUp(void)
{
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	int curExp = pPlayer->GetAttrib(CAB_EXP);
	int curLv = pPlayer->GetAttrib(CAB_LV);
	if(curLv >= 10)
		return false;

	if(curExp >= sLevelUpTable[curLv])
	{
		pPlayer->SetAttrib(CAB_LV, curLv + 1);
		pPlayer->SetAttrib(CAB_HPMAX, sHpMaxTable[curLv + 1]);
		int ranAdd;
		ranAdd = rand()%2 + 3;
		pPlayer->SetAttrib(CAB_AT, pPlayer->GetAttrib(CAB_AT) + ranAdd);
		ranAdd = rand()%2 + 3;
		pPlayer->SetAttrib(CAB_DF, pPlayer->GetAttrib(CAB_DF) + ranAdd);
		ranAdd = rand()%2 + 3;
		pPlayer->SetAttrib(CAB_STR, pPlayer->GetAttrib(CAB_STR) + ranAdd);
		ranAdd = rand()%2 + 3;
		pPlayer->SetAttrib(CAB_INT, pPlayer->GetAttrib(CAB_INT) + ranAdd);
		ranAdd = rand()%2 + 3;
		pPlayer->SetAttrib(CAB_AGL, pPlayer->GetAttrib(CAB_AGL) + ranAdd);
		ranAdd = rand()%2 + 3;
		pPlayer->SetAttrib(CAB_PHY, pPlayer->GetAttrib(CAB_PHY) + ranAdd);
		return true;
	}
	return false;
}

void BattleManager::SetupAttackAnimEffect(int index, float posX, float posY, float sizeFactor, int texId, int animMode, float scaleX)
{
	mpSprEffects[index]->SetPosition(posX, posY);
	mpSprEffects[index]->SetSize(36.0f*sizeFactor*scaleX, 30.0f*sizeFactor);
	mpSprEffects[index]->SetTexture(gTexMgr.GetBattleTex(texId));
	if(animMode == 0)
		mpSprEffects[index]->SetAnimator(2, 2, 4, 1, 20.0f, Sprite::ANIM_ONCE);
	else if(animMode == 1)
		mpSprEffects[index]->SetAnimator(4, 2, 8, 1, 30.0f, Sprite::ANIM_ONCE);
	else if(animMode == 2)
		mpSprEffects[index]->SetAnimator(2, 2, 4, 1, 40.0f, Sprite::ANIM_ONCE);
	else if(animMode == 3)
		mpSprEffects[index]->SetAnimator(4, 1, 4, 1, 30.0f, Sprite::ANIM_ONCE);
	mpSprEffects[index]->PlayAnim();
	mpSprEffects[index]->SetVisible(true);
}

void BattleManager::ClearAttackAnimEffects(void)
{
	for(int i=0; i<MAX_EFFECT_NUM; i++)
	{
		mpSprEffects[i]->SetVisible(false);
	}
}

void BattleManager::SetupAttackShotEffect(float posX, float posY, float sizeFactor, int texId, float targetX, float targetY, float speed)
{
	mpSprShot->SetPosition(posX, posY);
	mpSprShot->SetSize(36.0f*sizeFactor, 30.0f*sizeFactor);
	mpSprShot->SetTexture(gTexMgr.GetBattleTex(texId));
	mpSprShot->SetVisible(true);

	float lengthX = targetX - posX;
	float lengthY = targetY - posY;
	float factor = sqrt(lengthX*lengthX + lengthY*lengthY);
	assert(factor > 0);
	mbAnimShot = true;
	mShotDeltaX = 0.0f;
	mShotDeltaY = 0.0f;
	mShotInitDX = lengthX/factor;
	mShotInitDY = lengthY/factor;
	mShotSpeed = speed;
}

void BattleManager::ClearAttackShotEffect(void)
{
	mpSprShot->SetVisible(false);
	mbAnimShot = false;
	mShotDeltaX = 0.0f;
	mShotDeltaY = 0.0f;
	mShotInitDX = 0.0f;
	mShotInitDY = 0.0f;
	mShotSpeed = 0.0f;
}