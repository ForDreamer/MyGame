#include "BatUIAssistList.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "BatUITextBox.h"
#include "BatUIInfoBox.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../MapEngine/MapManager.h"
#include "../Manager/CharacterMgr.h"

BatUIAssistList::BatUIAssistList(void)
{
}

BatUIAssistList::~BatUIAssistList(void)
{
}

void BatUIAssistList::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectId = 0;

	const char* tempstr1[MAX_SELECT_ASSIST_NUM] = {"乘降","防御","逃跑"};
	for(int i=0; i<MAX_SELECT_ASSIST_NUM; i++)
	{
		mTxtEnemys[i].SetText(tempstr1[i]);
	}
	
}

void BatUIAssistList::CloseUI(void)
{
	UIBase::CloseUI();
}

void BatUIAssistList::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
}

void BatUIAssistList::Init(void)
{
	mSprBG.SetPosition(112.0f, -152.0f);
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<MAX_SELECT_ASSIST_NUM; i++)
	{
		mTxtEnemys[i].SetFontType(FT_BIGFONT);
		mTxtEnemys[i].SetPosition(0.0f, -90.0f - 32*i);
	}
}

void BatUIAssistList::Update(float baseX, float baseY)
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
				pUIMain = gBattleUIMgr.GetUI(BATUI_TANK_MAIN);
			pUIMain->SetOnFocus(true);

			FmodSound::PlaySnd(SND_CONFIRM);
			Global::ClearKeyState();
		}

		if(Global::KeyDown(KEY_UP) && mSelectId > 0)
		{
			mSelectId -= 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_DOWN) && mSelectId < MAX_SELECT_ASSIST_NUM-1)
		{
			mSelectId += 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
	}

	mSprBG.Update();
	mSprCursor.SetPosition(-30.0f, -108.0f - 32*mSelectId);
	mSprCursor.Update();
	for(int i=0; i<MAX_SELECT_ASSIST_NUM; i++)
	{
		mTxtEnemys[i].Update();
	}
}

void BatUIAssistList::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<MAX_SELECT_ASSIST_NUM; i++)
	{
		mTxtEnemys[i].Render();
	}
}

void BatUIAssistList::OnItemSelected(void)
{
	BatUITextBox* pUITextBox = (BatUITextBox*)gBattleUIMgr.GetUI(BATUI_TEXTBOX);
	BatUIInfoBox* pInfoBox = (BatUIInfoBox*)gBattleUIMgr.GetUI(BATUI_INFOBOX);

	switch(mSelectId)
	{
	case AST_LOAD:
		CloseUI();
		if(gMapMgr.IsPlayerInTank())
		{
			BattleActor* pPlayer = gBattleMgr.GetBatPlayer();
			char tempstr[16];

			if(gBattleMgr.GetBattleMode() == BM_HUMAN)
			{
				pUITextBox->OpenUI();
				sprintf_s(tempstr, "%s上车！", pPlayer->GetName());
				pUITextBox->SetLineText(0, tempstr);
				pUITextBox->SetIsOnLoad(true);
			}
			else if(gBattleMgr.GetBattleMode() == BM_TANK)
			{
				pUITextBox->OpenUI();
				sprintf_s(tempstr, "%s下车！", pPlayer->GetName());
				pUITextBox->SetLineText(0, tempstr);
				pUITextBox->SetIsOnLoad(true);
			}
		}
		else
		{
			pUITextBox->OpenUI();
			pUITextBox->SetLineText(0, "附近没有车！");
		}
		break;
	case AST_GUARD:
		CloseUI();
		pInfoBox->OpenUI();
		gBattleMgr.SetAction(BA_GUARD);
		gBattleMgr.StartAttackRound();
		break;
	case AST_ESCAPE:
		CloseUI();
		pInfoBox->OpenUI();
		gBattleMgr.SetAction(BA_ESCAPE);
		gBattleMgr.StartAttackRound();
		break;
	}
	FmodSound::PlaySnd(SND_CONFIRM);
}
