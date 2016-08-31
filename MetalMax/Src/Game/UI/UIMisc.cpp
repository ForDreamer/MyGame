#include "UIMisc.h"
#include "UIManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Engine/Scene/Scene.h"
#include "../MapEngine/MapManager.h"
#include "../Manager/DialogSystemMgr.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/BattleTankMgr.h"
#include "../Manager/GameStatusMgr.h"
#include "../Manager/ItemMgr.h"
#include "../Manager/EquipMgr.h"
#include "../Manager/SaveLoadMgr.h"
#include "../Scenes/MapNavigationScene.h"
#include "../Battle/BattleManager.h"
#include <strsafe.h>

UITextBox::UITextBox(void)
:	mCurLine(0),
	mCurSegment(0),
	mTotalLine(0),
	mTotalSegment(0),
	mSelectYesNo(SELECT_YES),
	mpTalkingActor(NULL),
	mbIntoInnScene(false),
	mbIntoWanterScene(false)
{
}

UITextBox::~UITextBox(void)
{
}

void UITextBox::ClearTxtBuffer(void)
{
	for(int i=0; i<MAX_LINES; i++)
	{
		for(int k=0; k<TXT_BUFFER; k++)
		{
			mStrTxts[i][k] = '\0';
		}
		mSegTargets[i] = -1;
		mTxtLines[i].SetText(mStrTxts[i]);
		mTxtLines[i].SetVisible(false);
	}

	mCurLine = 0;
	mCurSegment = 0;
	mTotalLine = 0;
	mTotalSegment = 0;
}

void UITextBox::UpdateVisibleLines(void)
{
	for(int i=0; i<MAX_LINES; i++)
	{
		if(i <= mSegTargets[mCurSegment])
		{
			mTxtLines[i].SetVisible(true);
		}
	}
}

void UITextBox::OpenUI(void)
{
	UIBase::OpenUI();
	ClearTxtBuffer();

	mSelectYesNo = SELECT_YES;
	mpTalkingActor = NULL;
	mbIntoInnScene = false;
	mbIntoWanterScene = false;
}

void UITextBox::CloseUI(void)
{
	UIBase::CloseUI();
	ClearTxtBuffer();
}

void UITextBox::SetText(const char* text)
{
	ClearTxtBuffer();

	int line = 0;
	int seg = 0;
	int bufPos = 0;
	for(int i=0; text[i] != '\0'; i++)
	{
		if(text[i] == '<')
		{
			mStrTxts[line][bufPos] = '\0';
			bufPos = 0;
			line++;
			mTotalLine++;
			continue;
		}

		if(text[i] == '@')
		{
			mSegTargets[seg] = line-1;
			seg++;
			mTotalSegment++;
			continue;
		}

		mStrTxts[line][bufPos] = text[i];
		bufPos++;
	}
	assert(mTotalLine > 0 && mTotalSegment > 0);

	UpdateVisibleLines();
	mCurLine = mSegTargets[mCurSegment];


	const DialogRecord& recordCur = gDialogSystemMgr.GetCurrentRecord();
	if((mCurSegment == mTotalSegment-1) && (recordCur.yesId || recordCur.noId))
	{
		SetTxtYesNoVisible(true);
		mSprCursor.SetVisible(false);
		mTxtYes.SetPosition(0.0f, -90.0f - mTotalLine*30.0f);
		mTxtNo.SetPosition(120.0f, -90.0f - mTotalLine*30.0f);
	}
	else
	{
		SetTxtYesNoVisible(false);
		mSprCursor.SetVisible(true);
	}
}

void UITextBox::Init(void)
{
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_TEXT_CURSOR));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	mSprCursor.SetAnimator(2, 1, 2, 1, 2.5f);
	mSprCursor.PlayAnim();

	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetText("");
		mTxtLines[i].SetPosition(-60.0f, -90.0f - i*30.0f);
	}
	mTxtYes.SetFontType(FT_BIGFONT);
	mTxtYes.SetText("是");
	mTxtNo.SetFontType(FT_BIGFONT);
	mTxtNo.SetText("否");

	mSprSelector.SetSize(36.0f, 30.0f);
	mSprSelector.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprSelector.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
}

void UITextBox::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			const DialogRecord& recordCur = gDialogSystemMgr.GetCurrentRecord();
			if(mCurSegment < mTotalSegment-1)
			{
				mCurSegment++;
				mCurLine = mSegTargets[mCurSegment];
				UpdateVisibleLines();

				if((mCurSegment == mTotalSegment-1) && (recordCur.yesId || recordCur.noId))
				{
					SetTxtYesNoVisible(true);
					mSprCursor.SetVisible(false);
					mTxtYes.SetPosition(0.0f, -90.0f - mTotalLine*30.0f);
					mTxtNo.SetPosition(120.0f, -90.0f - mTotalLine*30.0f);
				}
			}
			else
			{
				if(!recordCur.yesId && !recordCur.noId)
				{
					CloseUI();
					UIBase* pUI = gUIMgr.GetUI(UIT_MAIN);
					pUI->SetOnFocus(true);
					gUIMgr.CloseAllUI();

					//To Do: refactor dialog callback
					if(mpTalkingActor)
					{
						mpTalkingActor->RestoreTalkingDir();

						if(mpTalkingActor->GetActorID() == 35 && !gCharacterMgr.GetCharacter(CHARID_PLAYER)->IsMaxHP())
						{
							gMapMgr.SetNextMapInfo(MAPID_TOWN_HOME_3, 11, 15, 0);
							mbIntoInnScene = true;
						}

						if(mpTalkingActor->GetActorID() == 39 && recordCur.dialogId == 4)
						{
							if(gGameStatusMgr.GetGameState() == GS_FIND_TANK)
							{
								gGameStatusMgr.SetGameState(GS_END_GAME);
								FmodSound::PlaySnd(SND_OPENDOOR);
							}
						}

						if(mpTalkingActor->GetActorID() == 41)
						{
							gGameStatusMgr.SetGameState(GS_WIN_BOSS);
							MapNavigationScene::StartBattle();
							gBattleMgr.SetIsBossFight(true);
						}
					}
					if(mbIntoInnScene)
					{
						gSceneMgr.SetNextScene(SCENE_INN);
						FmodSound::StopMusic();
					}
					if(mbIntoWanterScene)
					{
						gSceneMgr.SetNextScene(SCENE_WANTER);
						MapActor* pPlayer = gMapMgr.GetActor(PLAYER_ID);
						gMapMgr.SetNextMapInfo(gMapMgr.GetCurMapID(), pPlayer->GetTileX(), pPlayer->GetTileY(), pPlayer->GetCurDir());
					}
				}
				else
				{
					const DialogRecordList& recordListCur = gDialogSystemMgr.GetCurrentRecordList();
					int nextId = -1;
					if(mSelectYesNo == SELECT_YES)
						nextId = recordCur.yesId;
					else if(mSelectYesNo == SELECT_NO)
						nextId = recordCur.noId;
					DialogRecord record = recordListCur[nextId];
					gDialogSystemMgr.SetCurrentRecord(record);
					SetText(record.text);
				}
			}
			Global::ClearKeyState();
		}

		if(mSprSelector.GetVisible())
		{
			if(Global::KeyDown(KEY_LEFT) && mSelectYesNo == SELECT_NO)
			{
				mSelectYesNo = SELECT_YES;
				FmodSound::PlaySnd(SND_SELECT);
			}
			if(Global::KeyDown(KEY_RIGHT) && mSelectYesNo == SELECT_YES)
			{
				mSelectYesNo = SELECT_NO;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
	}

	mSprBG.SetPosition(baseX+112.0f, baseY-152.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX+112, baseY-122.0f - mCurLine*30.0f);
	mSprCursor.Update();
	mSprSelector.SetPosition(baseX-20 + mSelectYesNo*120, baseY-110.0f - mTotalLine*30.0f);
	mSprSelector.Update();

	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].Update();
	}
	mTxtYes.Update();
	mTxtNo.Update();
}

void UITextBox::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	mSprSelector.Render();

	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].Render();
	}
	mTxtYes.Render();
	mTxtNo.Render();
}

void UITextBox::SetTxtYesNoVisible(bool visible)
{
	mTxtYes.SetVisible(visible);
	mTxtNo.SetVisible(visible);
	mSprSelector.SetVisible(visible);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
UISelector::UISelector(void)
:	mUser(UIS_MISC),
	mMaxItemNum(0)
{
}

UISelector::~UISelector(void)
{
}

void UISelector::OpenUI(void)
{
	UIBase::OpenUI();
	mSelectId = 0;
	mMaxItemNum = 0;
}

void UISelector::CloseUI(void)
{
	UIBase::CloseUI();
}

void UISelector::Init(void)
{
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetText("");
		mTxtLines[i].SetPosition(-20.0f, -90.0f - i*32.0f);
	}
}

void UISelector::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			CloseUI();
			Global::ClearKeyState();
			
			OnItemSelected();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_B))
		{
			CloseUI();
			UIBase* pUI = gUIMgr.GetUI(UIT_MAIN);
			pUI->SetOnFocus(true);

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_UP))
		{
			if(mSelectId > 0)
			{
				mSelectId--;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
		else if(Global::KeyDown(KEY_DOWN))
		{
			if(mSelectId < mMaxItemNum-1)
			{
				mSelectId++;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
	}

	mSprBG.SetPosition(baseX+112.0f, baseY-152.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX-45, baseY-105.0f - mSelectId*33.0f);
	mSprCursor.Update();
	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].Update();
	}
}

void UISelector::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].Render();
	}
}

void UISelector::OnItemSelected(void)
{
	UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
	UIBattleInfo* pUIBattleInfo = (UIBattleInfo*)(gUIMgr.GetUI(UIT_BATTLEINFO));

	switch(mUser)
	{
	case UIS_MISC:
		if(mSelectId == 0)
		{
			pUITextbox->OpenUI();
			pUITextbox->SetText("『 吱.吱..<@『 存储完毕！<@");
			gSaveLoadMgr.Save();
		}
		else if(mSelectId == 1)
		{
			gSceneMgr.SetNextScene(SCENE_STARTUP);
			gUIMgr.CloseAllUI();
		}
		break;
	case UIS_TOOL:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
UIBattleInfo::UIBattleInfo(void)
{
}

UIBattleInfo::~UIBattleInfo(void)
{
}

void UIBattleInfo::OpenUI(void)
{
	UIBase::OpenUI();
	
	int num1 = 10, num2 = 30, num3 = 200;
	char strtemp[3][32];
	for(int i=0; i<3; i++)
		for(int k=0; k<32; k++)
			strtemp[i][k] = '\0';
	sprintf_s(strtemp[0], "  仿生类          %d", num1);
	sprintf_s(strtemp[1], "  电子类          %d", num2);
	sprintf_s(strtemp[2], "  战车类          %d", num3);
	mTxtLines[0].SetText(strtemp[0], true);
	mTxtLines[1].SetText(strtemp[1], true);
	mTxtLines[2].SetText(strtemp[2], true);
}

void UIBattleInfo::CloseUI(void)
{
	UIBase::CloseUI();

	mTxtLines[0].ReleaseTextBuffer();
	mTxtLines[1].ReleaseTextBuffer();
	mTxtLines[2].ReleaseTextBuffer();
}

void UIBattleInfo::Init(void)
{
	mSprBG.SetSize(640.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TEXTBOX));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetText("");
		mTxtLines[i].SetPosition(-20.0f, -90.0f - i*32.0f);
	}
}

void UIBattleInfo::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A))
		{
			CloseUI();
			gUIMgr.CloseAllUI();

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}
	}

	mSprBG.SetPosition(baseX+112.0f, baseY-152.0f);
	mSprBG.Update();
	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].Update();
	}
}

void UIBattleInfo::Render(void)
{
	mSprBG.Render();
	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].Render();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
static char sTempInfoTxt[64] = "";

UITools::UITools(void)
:	mSelectRow(0),
	mSelectCol(0),
	mMaxItemNum(0)
{
}

UITools::~UITools(void)
{
}

void UITools::OpenUI(void)
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

void UITools::CloseUI(void)
{
	UIBase::CloseUI();
}

void UITools::Init(void)
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

void UITools::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_B))
		{
			CloseUI();
			UIBase* pUI = gUIMgr.GetUI(UIT_MAIN);
			pUI->SetOnFocus(true);

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_A))
		{
			CloseUI();
			UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
			pUITextbox->OpenUI();

			ApplyUseItemEffect(pUITextbox);

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
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

void UITools::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Render();
	}
}

void UITools::ApplyUseItemEffect(UITextBox* pUITextbox)
{
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	Item* pItem = gItemMgr.GetItem(mSelectId);

	// To Do: script the effect of item
	switch(pItem->GetID())
	{
	case ITEM_TOWEL:
		sprintf_s(sTempInfoTxt, "『 %s用%s擦汗,<@『 轻松了许多。<@", pPlayer->GetName(), pItem->GetName());
		pUITextbox->SetText(sTempInfoTxt);
		break;
	case ITEM_COIN:
		if(rand()%100 < 50)
			sprintf_s(sTempInfoTxt, "『 %s把%s弹出去,<@『 是正面！<@", pPlayer->GetName(), pItem->GetName());
		else
			sprintf_s(sTempInfoTxt, "『 %s把%s弹出去,<@『 是反面！<@", pPlayer->GetName(), pItem->GetName());
		pUITextbox->SetText(sTempInfoTxt);
		FmodSound::PlaySnd(SND_COIN);
		break;
	case ITEM_HP:
		pPlayer->AddHP(100);
		if(pPlayer->IsMaxHP())
			sprintf_s(sTempInfoTxt, "『 %s喝了%s！<@『 HP全满了！<@", pPlayer->GetName(), pItem->GetName());
		else
			sprintf_s(sTempInfoTxt, "『 %s喝了%s！<@『 HP恢复了100！<@", pPlayer->GetName(), pItem->GetName());
		pUITextbox->SetText(sTempInfoTxt);
		gItemMgr.RemoveItem(mSelectId);
		break;
	case ITEM_BOMB:
		sprintf_s(sTempInfoTxt, "『 %s使用了%s！<@『 但什么也没发生。<@", pPlayer->GetName(), pItem->GetName());
		pUITextbox->SetText(sTempInfoTxt);
		break;
	case ITEM_FIRE:
		sprintf_s(sTempInfoTxt, "『 %s使用了%s！<@『 但什么也没发生。<@", pPlayer->GetName(), pItem->GetName());
		pUITextbox->SetText(sTempInfoTxt);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
UICannon::UICannon(void)
{
}

UICannon::~UICannon(void)
{
}

void UICannon::OpenUI(void)
{
	UIBase::OpenUI();

	BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
	char strTemp[64];
	sprintf(strTemp, "主炮               45炮               %d/ %d", pTank->GetST(), pTank->GetSTMax());
	mTxtMain.SetText(strTemp, true);

	mTxtSpecial.SetText("特种弹                                   0/ 16");
}

void UICannon::CloseUI(void)
{
	UIBase::CloseUI();
}

void UICannon::Init(void)
{
	mSprBG.SetSize(642.0f, 576.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_CANNON));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mTxtMain.SetFontType(FT_BIGFONT);
	mTxtMain.SetPosition(-220.0f, 210.0f);
	mTxtSpecial.SetFontType(FT_BIGFONT);
	mTxtSpecial.SetPosition(-220.0f, 80.0f);
}

void UICannon::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A) || Global::KeyDown(KEY_B))
		{
			gUIMgr.CloseAllUI();
			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}
	}

	mSprBG.SetPosition(baseX, baseY+88.0f);
	mSprBG.Update();
	mTxtSpecial.Update();
	mTxtMain.Update();
}

void UICannon::Render(void)
{
	mSprBG.Render();
	mTxtSpecial.Render();
	mTxtMain.Render();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
UIEquip::UIEquip(void)
:	mSelectRow(0),
	mSelectCol(0),
	mMaxItemNum(0)
{
}

UIEquip::~UIEquip(void)
{
}

void UIEquip::OpenUI(void)
{
	UIBase::OpenUI();
	mSelectRow = 0;
	mSelectCol = 0;
	mSelectId = 0;
	mMaxItemNum = gEquipMgr.GetEquipNum();

	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].SetVisible(false);
		mEquipsPos[i] = -1;
	}

	for(int i=0; i<MAX_ITEMS; i++)
	{
		Equip* pEquip = gEquipMgr.GetEquip(i);
		if(pEquip)
			mTxtLines[i].SetText(pEquip->GetName());
		else
			mTxtLines[i].SetText("");

		if(pEquip && pEquip->IsEquiped())
		{
			EquipType type = pEquip->GetType();
			mSprEquips[type].SetVisible(true);
			mEquipsPos[type] = i;
		}
	}

	UpdateStatusText();
}

void UIEquip::CloseUI(void)
{
	UIBase::CloseUI();
}

void UIEquip::UpdateStatusText(void)
{
	char strTemp[32];
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	sprintf_s(strTemp, "攻击: %d", pPlayer->GetAttrib(CAB_AT));
	mTxtAT.SetText(strTemp, true);
	sprintf_s(strTemp, "防御: %d", pPlayer->GetAttrib(CAB_DF));
	mTxtDF.SetText(strTemp, true);
	sprintf_s(strTemp, "%s    Lv: %d", pPlayer->GetName(), pPlayer->GetAttrib(CAB_LV));
	mTxtLV.SetText(strTemp, true);
}

void UIEquip::Init(void)
{
	mSprBG.SetSize(642.0f, 576.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_CANNON));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].SetSize(20.0f, 15.0f);
		mSprEquips[i].SetTexture(gTexMgr.GetUITex(TEX_UI_GUN + i));
		mSprEquips[i].SetVisible(false);
	}

	float yIndex = 0.0f;
	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		if(i%2 == 0)
		{
			mTxtLines[i].SetPosition(-170.0f, 90.0f - yIndex*32.0f);
		}
		else
		{
			mTxtLines[i].SetPosition(70.0f, 90.0f - yIndex*32.0f);
			yIndex += 1.0f;
		}
	}
	mTxtLV.SetPosition(-220.0f, 210.0f);
	mTxtAT.SetPosition(-160.0f, 170.0f);
	mTxtDF.SetPosition(40.0f, 170.0f);
}

void UIEquip::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_B))
		{
			CloseUI();
			gUIMgr.CloseAllUI();

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_A))
		{
			Equip* pEquip = gEquipMgr.GetEquip(mSelectId);
			if(pEquip->IsEquiped())
			{
				pEquip->UnApplyEquip();
				EquipType type = pEquip->GetType();
				mSprEquips[type].SetVisible(false);
				mEquipsPos[type] = -1;
			}
			else
			{
				EquipType type = pEquip->GetType();
				if(mEquipsPos[type] == -1)
				{
					mSprEquips[type].SetVisible(true);
					mEquipsPos[type] = mSelectId;
					pEquip->ApplyEquip();
				}
				else
				{
					Equip* pEquipOld = gEquipMgr.GetEquip(mEquipsPos[type]);
					pEquipOld->UnApplyEquip();

					mSprEquips[type].SetVisible(true);
					mEquipsPos[type] = mSelectId;
					pEquip->ApplyEquip();
				}
			}

			UpdateStatusText();
			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
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

	mSprBG.SetPosition(baseX, baseY+88.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX-215 + 240.0f*mSelectCol, baseY+72.0f - 32.0f*mSelectRow);
	mSprCursor.Update();

	for(int i=0; i<ET_MAX; i++)
	{
		int row = mEquipsPos[i]/2;
		int col = mEquipsPos[i]%2;
		mSprEquips[i].SetPosition(baseX-185 + col*240.0f, baseY+72.0f - row*32.0f);
		mSprEquips[i].Update();
	}

	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Update();
	}
	mTxtLV.Update();
	mTxtAT.Update();
	mTxtDF.Update();
}

void UIEquip::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].Render();
	}

	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Render();
	}
	mTxtLV.Render();
	mTxtAT.Render();
	mTxtDF.Render();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
UIStatus::UIStatus(void)
{
}

UIStatus::~UIStatus(void)
{
}

void UIStatus::OpenUI(void)
{
	UIBase::OpenUI();

	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	mTxtName.SetText(pPlayer->GetName());

	char strTemp[32];
	sprintf_s(strTemp, "Lv:  %d", pPlayer->GetAttrib(CAB_LV));
	mTxtLV.SetText(strTemp, true);
	sprintf_s(strTemp, "HP:    %d/ %d", pPlayer->GetAttrib(CAB_HP), pPlayer->GetAttrib(CAB_HPMAX));
	mTxtHP.SetText(strTemp, true);
	sprintf_s(strTemp, "攻击:    %d", pPlayer->GetAttrib(CAB_AT));
	mTxtAT.SetText(strTemp, true);
	sprintf_s(strTemp, "防御:    %d", pPlayer->GetAttrib(CAB_DF));
	mTxtDF.SetText(strTemp, true);
	sprintf_s(strTemp, "力量:    %d", pPlayer->GetAttrib(CAB_STR));
	mTxtSTR.SetText(strTemp, true);
	sprintf_s(strTemp, "智力:    %d", pPlayer->GetAttrib(CAB_INT));
	mTxtINT.SetText(strTemp, true);
	sprintf_s(strTemp, "敏捷:    %d", pPlayer->GetAttrib(CAB_AGL));
	mTxtAG.SetText(strTemp, true);
	sprintf_s(strTemp, "体力:    %d", pPlayer->GetAttrib(CAB_PHY));
	mTxtPH.SetText(strTemp, true);

	sprintf_s(strTemp, "EXP:    %d", pPlayer->GetAttrib(CAB_EXP));
	mTxtEXP.SetText(strTemp, true);
	sprintf_s(strTemp, "GOLD:   %d", gGameStatusMgr.GetGoldNum());
	mTxtGOL.SetText(strTemp, true);

	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].SetVisible(false);
		mEquipsPos[i] = -1;
	}

	for(int i=0; i<MAX_ITEMS; i++)
	{
		Equip* pEquip = gEquipMgr.GetEquip(i);
		if(pEquip)
			mTxtLines[i].SetText(pEquip->GetName());
		else
			mTxtLines[i].SetText("");

		if(pEquip && pEquip->IsEquiped())
		{
			EquipType type = pEquip->GetType();
			mSprEquips[type].SetVisible(true);
			mEquipsPos[type] = i;
		}
	}
}

void UIStatus::CloseUI(void)
{
	UIBase::CloseUI();
}

void UIStatus::Init(void)
{
	mSprBG.SetSize(642.0f, 512.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_STATUS));
	mSprPlayer.SetSize(45.0f, 36.0f);
	mSprPlayer.SetTexture(gTexMgr.GetActorTex(TEX_ACT_PLAYER_WALK_R));
	mSprPlayer.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	mSprPlayer.SetAnimator(2, 1, 2, 1, 2.0f);
	mSprPlayer.PlayAnim();

	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].SetVisible(false);
		mSprEquips[i].SetSize(20.0f, 15.0f);
		mSprEquips[i].SetTexture(gTexMgr.GetUITex(TEX_UI_GUN + i));
	}
	
	mTxtName.SetPosition(-170.0f, 185.0f);
	mTxtLV.SetPosition(-170.0f, 155.0f);
	mTxtHP.SetPosition(-230.0f, 125.0f);

	mTxtAT.SetFontType(FT_BIGFONT);
	mTxtDF.SetFontType(FT_BIGFONT);
	mTxtSTR.SetFontType(FT_BIGFONT);
	mTxtINT.SetFontType(FT_BIGFONT);
	mTxtAG.SetFontType(FT_BIGFONT);
	mTxtPH.SetFontType(FT_BIGFONT);
	mTxtEXP.SetFontType(FT_BIGFONT);
	mTxtGOL.SetFontType(FT_BIGFONT);
	mTxtAT.SetPosition(70.0f, 170.0f);
	mTxtDF.SetPosition(70.0f, 130.0f);
	mTxtSTR.SetPosition(70.0f, 90.0f);
	mTxtINT.SetPosition(70.0f, 50.0f);
	mTxtAG.SetPosition(70.0f, 10.0f);
	mTxtPH.SetPosition(70.0f, -30.0f);
	mTxtEXP.SetPosition(70.0f, -110.0f);
	mTxtGOL.SetPosition(70.0f, -150.0f);

	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetPosition(-200.0f, 55.0f - i*32.0f);
	}
}

void UIStatus::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_A) || Global::KeyDown(KEY_B))
		{
			gUIMgr.CloseAllUI();
			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}
	}

	mSprBG.SetPosition(baseX, baseY);
	mSprBG.Update();
	mSprPlayer.SetPosition(baseX-210.0f, baseY+160.0f);
	mSprPlayer.Update();

	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].SetPosition(baseX-215, baseY+40.0f - mEquipsPos[i]*32.0f);
		mSprEquips[i].Update();
	}

	mTxtName.Update();
	mTxtLV.Update();
	mTxtHP.Update();

	mTxtAT.Update();
	mTxtDF.Update();
	mTxtSTR.Update();
	mTxtINT.Update();
	mTxtAG.Update();
	mTxtPH.Update();
	mTxtEXP.Update();
	mTxtGOL.Update();

	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Update();
	}
}

void UIStatus::Render(void)
{
	mSprBG.Render();
	mSprPlayer.Render();

	for(int i=0; i<ET_MAX; i++)
	{
		mSprEquips[i].Render();
	}

	mTxtName.Render();
	mTxtLV.Render();
	mTxtHP.Render();

	mTxtAT.Render();
	mTxtDF.Render();
	mTxtSTR.Render();
	mTxtINT.Render();
	mTxtAG.Render();
	mTxtPH.Render();
	mTxtEXP.Render();
	mTxtGOL.Render();

	for(int i=0; i<MAX_ITEMS; i++)
	{
		mTxtLines[i].Render();
	}
}