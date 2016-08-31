#include "BatUITextBox.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"

BatUITextBox::BatUITextBox(void)
{
}

BatUITextBox::~BatUITextBox(void)
{
}

void BatUITextBox::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectId = 0;
	mbIsOnLoad = false;
}

void BatUITextBox::CloseUI(void)
{
	UIBase::CloseUI();

	for(int i=0; i<MAX_TEXT_LINES; i++)
	{
		mTxtLines[i].SetText("");
		for(int k=0; k<MAX_TEXT_LENGTH; k++)
		{
			mTxtStrings[i][k] = '\0';
		}
	}
}

void BatUITextBox::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
}

void BatUITextBox::Init(void)
{
	mSprBG.SetPosition(112.0f, -152.0f);
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_TEXT_CURSOR));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	mSprCursor.SetAnimator(2, 1, 2, 1, 2.5f);
	mSprCursor.PlayAnim();

	for(int i=0; i<MAX_TEXT_LINES; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetPosition(-30.0f, -90.0f - 32*i);
	}
}

void BatUITextBox::SetLineText(int line, const char* text)
{
	assert(line < MAX_TEXT_LINES);

	strcpy_s(mTxtStrings[line], text);
	mTxtLines[line].SetText(mTxtStrings[line]);
	mSelectId = line;
}

void BatUITextBox::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			OnItemSelected();
			Global::ClearKeyState();
		}
	}

	mSprBG.Update();
	mSprCursor.SetPosition(112.0f, -122.0f - mSelectId*32.0f);
	mSprCursor.Update();
	for(int i=0; i<MAX_TEXT_LINES; i++)
	{
		mTxtLines[i].Update();
	}
}

void BatUITextBox::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<MAX_TEXT_LINES; i++)
	{
		mTxtLines[i].Render();
	}
}

void BatUITextBox::OnItemSelected(void)
{
	UIBase* pUIHumMain = gBattleUIMgr.GetUI(BATUI_HUMAN_MAIN);
	UIBase* pUITankMain = gBattleUIMgr.GetUI(BATUI_TANK_MAIN);

	CloseUI();
	if(mbIsOnLoad)
	{
		if(gBattleMgr.GetBattleMode() == BM_HUMAN)
		{
			pUIHumMain->CloseUI();
			pUITankMain->OpenUI();
			gBattleMgr.SetBattleMode(BM_TANK);
		}
		else if(gBattleMgr.GetBattleMode() == BM_TANK)
		{
			pUITankMain->CloseUI();
			pUIHumMain->OpenUI();
			gBattleMgr.SetBattleMode(BM_HUMAN);
		}
		FmodSound::PlaySnd(SND_GOTOTANK);
	}
	else
	{
		if(gBattleMgr.GetBattleMode() == BM_HUMAN)
			pUIHumMain->SetOnFocus(true);
		else if(gBattleMgr.GetBattleMode() == BM_TANK)
			pUITankMain->SetOnFocus(true);
	}
}
