#include "BatUIInfoBox.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"

BatUIInfoBox::BatUIInfoBox(void)
{
}

BatUIInfoBox::~BatUIInfoBox(void)
{
}

void BatUIInfoBox::ClearText()
{
	for(int i=0; i<MAX_INFO_LINES; i++)
	{
		mTxtLines[i].SetText("");
		for(int k=0; k<MAX_INFO_LENGTH; k++)
		{
			mTxtStrings[i][k] = '\0';
		}
	}
	mbShowCursor = false;
}

void BatUIInfoBox::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectId = 0;
	mbShowCursor = false;
}

void BatUIInfoBox::CloseUI(void)
{
	UIBase::CloseUI();
	ClearText();
}

void BatUIInfoBox::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
}

void BatUIInfoBox::Init(void)
{
	mSprBG.SetPosition(-100.0f, -152.0f);
	mSprBG.SetSize(642.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_BATUI_INFOBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_TEXT_CURSOR));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	mSprCursor.SetAnimator(2, 1, 2, 1, 2.5f);
	mSprCursor.PlayAnim();

	for(int i=0; i<MAX_INFO_LINES; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetPosition(-280.0f, -90.0f - 32*i);
	}
}

void BatUIInfoBox::SetLineText(int line, const char* text)
{
	assert(line < MAX_INFO_LINES);

	strcpy_s(mTxtStrings[line], text);
	mTxtLines[line].SetText(mTxtStrings[line]);
	mSelectId = line;

	mbShowCursor = true;
}

void BatUIInfoBox::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			if(!gBattleMgr.IsEnemyDieing() && !gBattleMgr.IsDelayingInput() && !gBattleMgr.IsAttacking())
			{
				OnItemSelected();
				Global::ClearKeyState();
			}
		}
	}

	mSprBG.Update();
	mSprCursor.SetPosition(-92.0f, -122.0f - mSelectId*32.0f);
	mSprCursor.Update();
	for(int i=0; i<MAX_INFO_LINES; i++)
	{
		mTxtLines[i].Update();
	}
}

void BatUIInfoBox::Render(void)
{
	mSprBG.Render();
	if(mbShowCursor)
		mSprCursor.Render();
	for(int i=0; i<MAX_INFO_LINES; i++)
	{
		mTxtLines[i].Render();
	}
}

void BatUIInfoBox::OnItemSelected(void)
{
	ClearText();
	BattleState batState = gBattleMgr.GetState();

	if(gBattleMgr.IsExitBattle())
	{
		FmodSound::StopMusic();
		if(batState == BS_FAIL)
		{
			FmodSound::StopSnd(SND_FAIL);
			gSceneMgr.SetNextScene(SCENE_STARTUP);
		}
		else
			gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
		return;
	}
	
	if(batState == BS_ATTACK)
	{
		if(gBattleMgr.IsLastAttacker())
		{
			UIBase* pUIHumMain = gBattleUIMgr.GetUI(BATUI_HUMAN_MAIN);
			UIBase* pUITankMain = gBattleUIMgr.GetUI(BATUI_TANK_MAIN);

			CloseUI();
			if(gBattleMgr.GetBattleMode() == BM_HUMAN)
				pUIHumMain->SetOnFocus(true);
			else if(gBattleMgr.GetBattleMode() == BM_TANK)
				pUITankMain->SetOnFocus(true);
			gBattleMgr.FinishAttackRound();
		}
		else
		{
			gBattleMgr.ResumeAttack();
		}
	}
	else if(batState == BS_WINNING)
	{
		if(gBattleMgr.IsLevelUp())
			gBattleMgr.StartLevelUp();
	}
}
