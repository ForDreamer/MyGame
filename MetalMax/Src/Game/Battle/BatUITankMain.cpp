#include "BatUITankMain.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "BattleActor.h"
#include "BatUITextBox.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../Manager/BattleTankMgr.h"

static const float sCursorX[BAT_UIT_MAX] = {-285.0f, -185.0f, -285.0f, -185.0f};
static const float sCursorY[BAT_UIT_MAX] = {-145.0f, -145.0f, -176.0f, -176.0f};
static char sStrHP[16];
static char sStrSP[16];

BatUITankMain::BatUITankMain(void)
{
}

BatUITankMain::~BatUITankMain(void)
{
}

void BatUITankMain::OpenUI(void)
{
	UIBase::OpenUI();
	ClearSprHints();

	mSelectId = 0;

	BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
	mTxtTankName.SetText(pTank->GetName());

	BattleActor* pBatPlayer = gBattleMgr.GetBatPlayer();
	mTxtPlayerName.SetText(pBatPlayer->GetName());
	sprintf_s(sStrHP, "HP   %d", pBatPlayer->GetHP());
	mTxtPlayerHp.SetText(sStrHP);
	sprintf_s(sStrSP, "SP   %d", pBatPlayer->GetSP());
	mTxtTankSp.SetText(sStrSP);

	SetInfoBarText(pTank->GetName());
}

void BatUITankMain::CloseUI(void)
{
	UIBase::CloseUI();
}

void BatUITankMain::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
	if(focus)
		ClearSprHints();
}

void BatUITankMain::Init(void)
{
	mSprBG.SetPosition(0.0f, -152.0f);
	mSprBG.SetSize(642.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_BATUI_TANK_MAIN));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<BAT_UIT_MAX; i++)
	{
		mSprHints[i].SetPosition(sCursorX[i] + 45.0f, sCursorY[i] + 1.0f);
		mSprHints[i].SetSize(80.0f, 32.0f);
		mSprHints[i].SetTexture(gTexMgr.GetUITex(TEX_BATUI_ITEMTANK_0 + i));
		mSprHints[i].SetVisible(false);
	}

	mTxtInfoBar.SetFontType(FT_SMALLFONT);
	mTxtInfoBar.SetPosition(-270.0f, -88.0f);
	mTxtPlayerName.SetFontType(FT_SMALLFONT);
	mTxtPlayerName.SetPosition(-40.0f, -90.0f);
	mTxtPlayerHp.SetFontType(FT_SMALLFONT);
	mTxtPlayerHp.SetPosition(170.0f, -90.0f);

	mTxtTankName.SetFontType(FT_SMALLFONT);
	mTxtTankName.SetPosition(-40.0f, -110.0f);
	mTxtTankSp.SetFontType(FT_SMALLFONT);
	mTxtTankSp.SetPosition(170.0f, -110.0f);
}

void BatUITankMain::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			OnItemSelected();
			Global::ClearKeyState();
		}

		if(Global::KeyDown(KEY_UP) && mSelectId >= 2)
		{
			mSelectId -= 2;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_DOWN) && mSelectId <= 1)
		{
			mSelectId += 2;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_LEFT) && mSelectId % 2 == 1)
		{
			mSelectId -= 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_RIGHT) && mSelectId % 2 == 0)
		{
			mSelectId += 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
	}

	mSprBG.Update();
	for(int i=0; i<BAT_UIT_MAX; i++)
	{
		mSprHints[i].Render();
	}
	mSprCursor.SetPosition(sCursorX[mSelectId], sCursorY[mSelectId]);
	mSprCursor.Update();
	
	mTxtInfoBar.Update();
	mTxtPlayerName.Update();
	mTxtPlayerHp.Update();
	mTxtTankName.Update();
	mTxtTankSp.Update();

	BattleActor* pBatPlayer = gBattleMgr.GetBatPlayer();
	sprintf_s(sStrHP, "HP   %d", pBatPlayer->GetHP());
	mTxtPlayerHp.SetText(sStrHP);
	sprintf_s(sStrSP, "SP   %d", pBatPlayer->GetSP());
	mTxtTankSp.SetText(sStrSP);
}

void BatUITankMain::Render(void)
{
	mSprBG.Render();
	for(int i=0; i<BAT_UIT_MAX; i++)
	{
		mSprHints[i].Render();
	}
	mSprCursor.Render();

	mTxtInfoBar.Render();
	mTxtPlayerName.Render();
	mTxtPlayerHp.Render();
	mTxtTankName.Render();
	mTxtTankSp.Render();
}

void BatUITankMain::OnItemSelected(void)
{
	UIBase* pUIAtkList = gBattleUIMgr.GetUI(BATUI_ATK_LIST);
	UIBase* pUIAssistList = gBattleUIMgr.GetUI(BATUI_ASSIST_LIST);
	BatUITextBox* pUITextBox = (BatUITextBox*)gBattleUIMgr.GetUI(BATUI_TEXTBOX);
	
	switch(mSelectId)
	{
	case BAT_UIT_ATK:
		{
			char tempstr[64];
			BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
			int shotNum = pTank->GetST();
			if(shotNum == 0)
			{
				SetOnFocus(false);
				pUITextBox->OpenUI();
				pUITextBox->SetLineText(0, "没有炮弹！");
			}
			else
			{
				sprintf(tempstr, "弹仓    %d", shotNum);
				SetInfoBarText(tempstr);
				SetOnFocus(false);
				pUIAtkList->OpenUI();
			}
		}
		break;
	case BAT_UIT_ATK_SUB:
		SetOnFocus(false);
		pUITextBox->OpenUI();
		pUITextBox->SetLineText(0, "副炮没有装备！");
		break;
	case BAT_UIT_SPECIAL:
		SetOnFocus(false);
		pUITextBox->OpenUI();
		pUITextBox->SetLineText(0, "没有特殊炮弹！");
		break;
	case BAT_UIT_ASSIST:
		SetOnFocus(false);
		pUIAssistList->OpenUI();
		break;
	}

	mSprHints[mSelectId].SetVisible(!mSprHints[mSelectId].GetVisible());
	FmodSound::PlaySnd(SND_CONFIRM);
}

void BatUITankMain::ClearSprHints(void)
{
	for(int i=0; i<BAT_UIT_MAX; i++)
	{
		mSprHints[i].SetVisible(false);
	}
}