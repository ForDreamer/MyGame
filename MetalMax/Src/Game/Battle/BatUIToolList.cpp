#include "BatUIToolList.h"
#include "BattleUIMgr.h"
#include "BattleManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../Manager/ItemMgr.h"

BatUIToolList::BatUIToolList(void)
:	mSelectRow(0),
	mSelectCol(0),
	mMaxItemNum(0)
{
}

BatUIToolList::~BatUIToolList(void)
{
}

void BatUIToolList::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectRow = 0;
	mSelectCol = 0;
	mSelectId = 0;
	
	mMaxItemNum = gItemMgr.GetItemNum();
	for(int i=0; i<MAX_ITEMS; i++)
	{
		Item* pItem = gItemMgr.GetItem(i);
		if(pItem)
		{
			mTxtLines[i].SetText(pItem->GetName());
		}
		else
		{
			mTxtLines[i].SetText("");
		}
	}
}

void BatUIToolList::CloseUI(void)
{
	UIBase::CloseUI();
}

void BatUIToolList::Init(void)
{
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	float yIndex = 0.0f;
	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetText("");
		if(i%2 == 0)
		{
			mTxtLines[i].SetPosition(-20.0f, -90.0f - yIndex*32.0f);
		}
		else
		{
			mTxtLines[i].SetPosition(140.0f, -90.0f - yIndex*32.0f);
			yIndex += 1.0f;
		}
	}
}

void BatUIToolList::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_B))
		{
			CloseUI();
			UIBase* pUIMain = gBattleUIMgr.GetUI(BATUI_HUMAN_MAIN);
			pUIMain->SetOnFocus(true);

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_A))
		{
			OnItemSelected();
			Global::ClearKeyState();
		}

		if(Global::KeyDown(KEY_UP))
		{
			if(mSelectRow > 0)
			{
				mSelectRow--;
				mSelectId -= 2;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
		else if(Global::KeyDown(KEY_DOWN))
		{
			if(mSelectRow < 3 && mSelectId+2 < mMaxItemNum)
			{
				mSelectRow++;
				mSelectId += 2;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
		else if(Global::KeyDown(KEY_LEFT))
		{
			if(mSelectCol > 0)
			{
				mSelectCol--;
				mSelectId -= 1;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
		else if(Global::KeyDown(KEY_RIGHT))
		{
			if(mSelectCol < 1 && mSelectId+1 < mMaxItemNum)
			{
				mSelectCol++;
				mSelectId += 1;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
	}

	mSprBG.SetPosition(baseX+112.0f, baseY-152.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX-40 + 160.0f*mSelectCol, baseY-112.0f - 32.0f*mSelectRow);
	mSprCursor.Update();
	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Update();
	}
}

void BatUIToolList::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Render();
	}
}

void BatUIToolList::OnItemSelected(void)
{
	ItemID itemId = gItemMgr.GetItem(mSelectId)->GetID();
	assert(itemId != ITEM_INVALID);
	gBattleMgr.SetUseToolID(itemId);
	
	if(itemId == ITEM_BOMB || itemId == ITEM_FIRE)
	{
		CloseUI();
		UIBase* pUIAtkList = gBattleUIMgr.GetUI(BATUI_ATK_LIST);
		pUIAtkList->OpenUI();

		gItemMgr.RemoveItem(mSelectId);
	}
	else
	{
		gBattleMgr.SetAction(BA_TOOL);
		gBattleMgr.StartAttackRound();

		CloseUI();
		UIBase* pInfoBox = gBattleUIMgr.GetUI(BATUI_INFOBOX);
		pInfoBox->OpenUI();

		if(itemId == ITEM_HP)
			gItemMgr.RemoveItem(mSelectId);
	}
	FmodSound::PlaySnd(SND_CONFIRM);
}