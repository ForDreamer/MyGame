#include "BatUIAtkList.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "BatUITankMain.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../Manager/BattleTankMgr.h"

BatUIAtkList::BatUIAtkList(void)
:	mMaxSelectNum(0)
{
}

BatUIAtkList::~BatUIAtkList(void)
{
}

void BatUIAtkList::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectId = 0;
	mMaxSelectNum = gBattleMgr.GetEnemyNum();
	BatEnemyList& enemyList = gBattleMgr.GetEnemyList();

	for(int i=0; i<MAX_SELECT_NUM; i++)
	{
		if(i < mMaxSelectNum)
			mTxtEnemys[i].SetText(enemyList.GetEnemy(i)->GetName());
		else
			mTxtEnemys[i].SetText("");
	}
}

void BatUIAtkList::CloseUI(void)
{
	UIBase::CloseUI();
}

void BatUIAtkList::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
}

void BatUIAtkList::Init(void)
{
	mSprBG.SetPosition(112.0f, -152.0f);
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<MAX_SELECT_NUM; i++)
	{
		mTxtEnemys[i].SetFontType(FT_BIGFONT);
		mTxtEnemys[i].SetPosition(0.0f, -90.0f - 32*i);
	}
}

void BatUIAtkList::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			OnItemSelected();
			Global::ClearKeyState();
		}

		if(Global::KeyDown(KEY_B))
		{
			CloseUI();
			UIBase* pUIMain = NULL;
			if(gBattleMgr.GetBattleMode() == BM_HUMAN)
				pUIMain = gBattleUIMgr.GetUI(BATUI_HUMAN_MAIN);
			else if(gBattleMgr.GetBattleMode() == BM_TANK)
			{
				pUIMain = gBattleUIMgr.GetUI(BATUI_TANK_MAIN);
				BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
				((BatUITankMain*)pUIMain)->SetInfoBarText(pTank->GetName());
			}
			pUIMain->SetOnFocus(true);

			FmodSound::PlaySnd(SND_CONFIRM);
			Global::ClearKeyState();
		}

		if(Global::KeyDown(KEY_UP) && mSelectId > 0)
		{
			mSelectId -= 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_DOWN) && mSelectId < mMaxSelectNum-1)
		{
			mSelectId += 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
	}

	mSprBG.Update();
	mSprCursor.SetPosition(-30.0f, -108.0f - 32*mSelectId);
	mSprCursor.Update();
	for(int i=0; i<mMaxSelectNum; i++)
	{
		mTxtEnemys[i].Update();
	}
}

void BatUIAtkList::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<mMaxSelectNum; i++)
	{
		mTxtEnemys[i].Render();
	}
}

void BatUIAtkList::OnItemSelected(void)
{
	gBattleMgr.SetAction(BA_ATTACK);
	gBattleMgr.SetAttackID(mSelectId);
	gBattleMgr.StartAttackRound();

	FmodSound::PlaySnd(SND_CONFIRM);
	CloseUI();
	UIBase* pInfoBox = gBattleUIMgr.GetUI(BATUI_INFOBOX);
	pInfoBox->OpenUI();

	if(gBattleMgr.GetBattleMode() == BM_TANK)
	{
		BatUITankMain* pUITankMain = (BatUITankMain*)gBattleUIMgr.GetUI(BATUI_TANK_MAIN);
		BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
		pTank->SetST(pTank->GetST() - 1);
		pUITankMain->SetInfoBarText(pTank->GetName());
	}
}
