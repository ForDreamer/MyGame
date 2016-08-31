#include "BatUIHumanMain.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "BattleActor.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"

static const float sCursorX[BAT_UIM_MAX] = {-285.0f, -185.0f, -285.0f};
static const float sCursorY[BAT_UIM_MAX] = {-145.0f, -145.0f, -176.0f};
static char sStrHP[16];

BatUIHumanMain::BatUIHumanMain(void)
{
}

BatUIHumanMain::~BatUIHumanMain(void)
{
}

void BatUIHumanMain::OpenUI(void)
{
	UIBase::OpenUI();
	ClearSprHints();

	mSelectId = 0;

	BattleActor* pBatPlayer = gBattleMgr.GetBatPlayer();
	mTxtPlayerName.SetText(pBatPlayer->GetName());

	SetInfoBarText(pBatPlayer->GetName());
}

void BatUIHumanMain::CloseUI(void)
{
	UIBase::CloseUI();
}

void BatUIHumanMain::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
	if(focus)
		ClearSprHints();
}

void BatUIHumanMain::Init(void)
{
	mSprBG.SetPosition(0.0f, -152.0f);
	mSprBG.SetSize(642.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_BATUI_HUM_MAIN));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<BAT_UIM_MAX; i++)
	{
		mSprHints[i].SetPosition(sCursorX[i] + 45.0f, sCursorY[i] + 1.0f);
		mSprHints[i].SetSize(80.0f, 32.0f);
		mSprHints[i].SetTexture(gTexMgr.GetUITex(TEX_BATUI_ITEMHUM_0 + i));
		mSprHints[i].SetVisible(false);
	}

	mTxtInfoBar.SetFontType(FT_SMALLFONT);
	mTxtInfoBar.SetPosition(-270.0f, -88.0f);
	mTxtPlayerName.SetFontType(FT_SMALLFONT);
	mTxtPlayerName.SetPosition(-40.0f, -90.0f);
	mTxtPlayerHp.SetFontType(FT_SMALLFONT);
	mTxtPlayerHp.SetPosition(170.0f, -90.0f);
}

void BatUIHumanMain::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			OnItemSelected();
			Global::ClearKeyState();
		}

		if(Global::KeyDown(KEY_UP) && mSelectId == 2)
		{
			mSelectId = 0;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_DOWN) && mSelectId == 0)
		{
			mSelectId = 2;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_LEFT) && mSelectId == 1)
		{
			mSelectId = 0;
			FmodSound::PlaySnd(SND_SELECT);
		}
		else if(Global::KeyDown(KEY_RIGHT) && mSelectId == 0)
		{
			mSelectId = 1;
			FmodSound::PlaySnd(SND_SELECT);
		}
	}

	mSprBG.Update();
	for(int i=0; i<BAT_UIM_MAX; i++)
	{
		mSprHints[i].Render();
	}
	mSprCursor.SetPosition(sCursorX[mSelectId], sCursorY[mSelectId]);
	mSprCursor.Update();
	
	mTxtInfoBar.Update();
	mTxtPlayerName.Update();
	mTxtPlayerHp.Update();

	BattleActor* pBatPlayer = gBattleMgr.GetBatPlayer();
	sprintf_s(sStrHP, "HP   %d", pBatPlayer->GetHP());
	mTxtPlayerHp.SetText(sStrHP);
}

void BatUIHumanMain::Render(void)
{
	mSprBG.Render();
	for(int i=0; i<BAT_UIM_MAX; i++)
	{
		mSprHints[i].Render();
	}
	mSprCursor.Render();

	mTxtInfoBar.Render();
	mTxtPlayerName.Render();
	mTxtPlayerHp.Render();
}

void BatUIHumanMain::OnItemSelected(void)
{
	UIBase* pUIAtkList = gBattleUIMgr.GetUI(BATUI_ATK_LIST);
	UIBase* pUIToolList = gBattleUIMgr.GetUI(BATUI_TOOL_LIST);
	UIBase* pUIAssistList = gBattleUIMgr.GetUI(BATUI_ASSIST_LIST);

	switch(mSelectId)
	{
	case BAT_UIM_ATTACK:
		SetOnFocus(false);
		pUIAtkList->OpenUI();
		break;
	case BAT_UIM_TOOL:
		SetOnFocus(false);
		pUIToolList->OpenUI();
		break;
	case BAT_UIM_ASSIST:
		SetOnFocus(false);
		pUIAssistList->OpenUI();
		break;
	}

	mSprHints[mSelectId].SetVisible(!mSprHints[mSelectId].GetVisible());
	FmodSound::PlaySnd(SND_CONFIRM);
}

void BatUIHumanMain::ClearSprHints(void)
{
	for(int i=0; i<BAT_UIM_MAX; i++)
	{
		mSprHints[i].SetVisible(false);
	}
}