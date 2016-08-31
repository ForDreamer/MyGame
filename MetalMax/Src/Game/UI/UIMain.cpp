#include "UIMain.h"
#include "UIMisc.h"
#include "UITop.h"
#include "UIManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Engine/Scene/Scene.h"
#include "../Scenes/NameSettingScene.h"
#include "../MapEngine/MapManager.h"
#include "../Manager/DialogSystemMgr.h"
#include "../Manager/GameStatusMgr.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/BattleTankMgr.h"
#include "../Manager/ItemMgr.h"
#include "../Manager/EquipMgr.h"

UIMain::UIMain(void)
:	mSelectRow(0),
	mSelectCol(0)
{
}

UIMain::~UIMain(void)
{
}

void UIMain::OpenUI(void)
{
	UIBase::OpenUI();
	ClearSprHints();

	mSelectRow = 0;
	mSelectCol = 0;
	mSelectId = 0;

	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	mTxtName.SetText(pPlayer->GetName());

	char strTemp[32];
	sprintf_s(strTemp, "HP   %d", pPlayer->GetAttrib(CAB_HP));
	mTxtPlayerInfo.SetText(strTemp, true);

	if(gMapMgr.IsPlayerInTank())
	{
		BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
		sprintf_s(strTemp, "SP   %d", pTank->GetSP());
		mTxtTankInfo.SetText(strTemp, true);
		mTxtTankName.SetText(pTank->GetName());
	}
	else
	{
		mTxtTankInfo.SetText("");
		mTxtTankName.SetText("");
	}
}

void UIMain::CloseUI(void)
{
	UIBase::CloseUI();
}

void UIMain::SetOnFocus(bool focus)
{
	UIBase::SetOnFocus(focus);
	ClearSprHints();
}

void UIMain::Init(void)
{
	mSprBG.SetSize(642.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_MAIN));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<MAX_UIM_ITEMS; i++)
	{
		mSprHint[i].SetSize(80.0f, 32.0f);
		mSprHint[i].SetTexture(gTexMgr.GetUITex(TEX_UI_ITEM_0 + i));
		mSprHint[i].SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	}

	mTxtPlayerInfo.SetFontType(FT_SMALLFONT);
	mTxtPlayerInfo.SetPosition(170.0f, -90.0f);
	mTxtTankInfo.SetFontType(FT_SMALLFONT);
	mTxtTankInfo.SetPosition(170.0f, -110.0f);

	mTxtName.SetFontType(FT_SMALLFONT);
	mTxtName.SetPosition(-40.0f, -90.0f);
	mTxtTankName.SetFontType(FT_SMALLFONT);
	mTxtTankName.SetPosition(-40.0f, -110.0f);
}

void UIMain::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_B))
		{
			CloseUI();
			mSelectId = 0;
			gUIMgr.SetActive(false);
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
			if(mSelectRow < 3)
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
			if(mSelectCol < 1)
			{
				mSelectCol++;
				mSelectId += 1;
				FmodSound::PlaySnd(SND_SELECT);
			}
		}
	}

	mSprBG.SetPosition(baseX, baseY-152.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX-285 + 100.0f*mSelectCol, baseY-112.0f - 32.0f*mSelectRow);
	mSprCursor.Update();
	float yIndex = 0.0f;
	for(int i=0; i<MAX_UIM_ITEMS; i++)
	{
		if(i%2 == 0)
		{
			mSprHint[i].SetPosition(baseX-240, baseY-112.0f - 32.5f*yIndex);
		}
		else
		{
			mSprHint[i].SetPosition(baseX-140, baseY-112.0f - 32.5f*yIndex);
			yIndex += 1.0f;
		}
		mSprHint[i].Update();
	}
	mTxtName.Update();
	mTxtTankName.Update();
	mTxtPlayerInfo.Update();
	mTxtTankInfo.Update();
}

void UIMain::Render(void)
{
	mSprBG.Render();
	for(int i=0; i<MAX_UIM_ITEMS; i++)
	{
		mSprHint[i].Render();
	}
	mSprCursor.Render();
	mTxtName.Render();
	mTxtTankName.Render();
	mTxtPlayerInfo.Render();
	mTxtTankInfo.Render();
}

void UIMain::OnItemSelected(void)
{
	UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
	UISelector* pUISelector = (UISelector*)(gUIMgr.GetUI(UIT_SELECTOR));
	UITools* pUITools = (UITools*)(gUIMgr.GetUI(UIT_TOOLS));
	UICannon* pUICannon = (UICannon*)(gUIMgr.GetUI(UIT_CANNON));
	UIEquip* pUIEquip = (UIEquip*)(gUIMgr.GetUI(UIT_EQUIP));
	UIStatus* pUIStatus = (UIStatus*)(gUIMgr.GetUI(UIT_STATUS));

	switch(mSelectId)
	{
	case UIM_TALK:
		ProcessOnTalk();
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	case UIM_LOAD:
		pUITextbox->OpenUI();
		if(gMapMgr.IsPlayerCanGotoTank())
		{
			if(gGameStatusMgr.GetGameState() == GS_WIN_BOSS)
			{
				gSceneMgr.SetNextScene(SCENE_NAMESETTING);
				gGameStatusMgr.SetGameState(GS_FIND_TANK);
				NameSettingScene::sUserType = UT_TANK;
				gUIMgr.CloseAllUI();
				FmodSound::PlaySnd(SND_GOTOTANK);
				gMapMgr.SetNextMapInfo(MAPID_CAVE_3, 31, 32, 0);
			}
			else
			{
				if(gMapMgr.IsPlayerInTank())
				{
					pUITextbox->SetText("『 下车！<@");
					gMapMgr.PlayerGotoTank(false);
				}
				else
				{
					pUITextbox->SetText("『 上车！<@");
					gMapMgr.PlayerGotoTank(true);
				}
				FmodSound::PlaySnd(SND_GOTOTANK);
			}
		}
		else
		{
			pUITextbox->SetText("『 那儿不能坐车！<@");
			FmodSound::PlaySnd(SND_CONFIRM);
		}
		break;
	case UIM_POWER:
		pUIStatus->OpenUI();
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	case UIM_TOOL:
		pUITools->OpenUI();
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	case UIM_EQUIP:
		pUIEquip->OpenUI();
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	case UIM_CANNON:
		if(gMapMgr.IsPlayerInTank())
			pUICannon->OpenUI();
		else
		{
			pUITextbox->OpenUI();
			pUITextbox->SetText("『 没带车！<@");
		}
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	case UIM_DETECT:
		ProcessOnDetect();
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	case UIM_MISC:
		pUISelector->OpenUI();
		pUISelector->SetUser(UISelector::UIS_MISC);
		pUISelector->SetMaxItemNum(2);
		pUISelector->SetLineText(0, "存储游戏");
		pUISelector->SetLineText(1, "离开游戏");
		FmodSound::PlaySnd(SND_CONFIRM);
		break;
	}
	SetOnFocus(false);
	ClearSprHints();
	mSprHint[mSelectId].SetVisible(true);
}

void UIMain::ClearSprHints(void)
{
	for(int i=0; i<MAX_UIM_ITEMS; i++)
	{
		mSprHint[i].SetVisible(false);
	}
}

void UIMain::ProcessOnTalk(void)
{
	UITop* pUITop = (UITop*)(gUIMgr.GetUI(UIT_TOP));
	UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
	pUITextbox->OpenUI();

	int actorId = gMapMgr.GetTargetActorId();
	MapActor* pActor = gMapMgr.GetActor(actorId);
	if(pActor)
	{
		MapActor::MoveDir talkDir = gMapMgr.GetActorTalkingDir(pActor);
		pActor->SetTalkingDir(talkDir);
		pUITextbox->SetTalkingActor(pActor);
	}

	DialogRecordList recordList = gDialogSystemMgr.GetDialogRecordList(actorId);
	gDialogSystemMgr.SetCurrentRecordList(recordList);
	if(recordList.size() > 0)
	{
		int gamestate = gGameStatusMgr.GetGameState();
		DialogRecord* pRecord = gDialogSystemMgr.GetDialogRecordByState(gamestate);
		int prevstate = gamestate;
		while(pRecord == NULL && prevstate >= 0)
		{
			prevstate--;
			pRecord = gDialogSystemMgr.GetDialogRecordByState(prevstate);
		}
		assert(pRecord);
		gDialogSystemMgr.SetCurrentRecord(*pRecord);
		pUITextbox->SetText(pRecord->text);
	}
	else
	{
		if(actorId == 10)
		{
			pUITop->SetUIType(UITOP_HERO);
			pUITop->OpenUI();

			pUITextbox->SetText("『 欢迎光临<    勇士办事处。<@");
			pUITextbox->SetOnFocus(false);
		}
		else if(actorId == 13)
		{
			pUITop->SetUIType(UITOP_EQUIP);
			pUITop->OpenUI();

			pUITextbox->SetText("『 欢迎光临<    武器店。<@");
			pUITextbox->SetOnFocus(false);
		}
		else if(actorId == 14)
		{
			pUITop->SetUIType(UITOP_TOOL);
			pUITop->OpenUI();

			pUITextbox->SetText("『 请进人类<    工具店。<@");
			pUITextbox->SetOnFocus(false);
		}
		else if(actorId == 20)
		{
			pUITop->SetUIType(UITOP_BAR);
			pUITop->OpenUI();

			pUITextbox->SetText("『 喝点什么？<@");
			pUITextbox->SetOnFocus(false);
		}
		else if(actorId == 22)
		{
			pUITop->SetUIType(UITOP_INN);
			pUITop->OpenUI();

			pUITextbox->SetText("『 欢迎光临旅馆。<    住哪个房间呢？<@");
			pUITextbox->SetOnFocus(false);
		}
		else if(actorId == 31)
		{
			pUITop->SetUIType(UITOP_CANNON);
			pUITop->OpenUI();

			pUITextbox->SetText("『 欢迎光临战车店。<    补充什么？<@");
			pUITextbox->SetOnFocus(false);
		}
		else if(actorId == 35)
		{
			if(gCharacterMgr.GetCharacter(CHARID_PLAYER)->IsMaxHP())
				pUITextbox->SetText("姐姐『 加油！保重。<@");
			else
				pUITextbox->SetText("姐姐『 终于回来了！<@姐姐『 累了吧？<    休息一下吧。<@");
		}
		else if(actorId == 41)
		{
			pUITextbox->SetText("战狗『 汪！汪！<@");
		}
		else
		{
			pUITextbox->SetText("『 眼前没人。<@");
		}
	}
}

static char sStrTempDetect[64];
void UIMain::ProcessOnDetect(void)
{
	UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
	pUITextbox->OpenUI();
	Character* pCharacter = gCharacterMgr.GetCharacter(CHARID_PLAYER);

	MapDetector* pDetector = gMapMgr.GetTargetDetector();
	if(pDetector && !pDetector->IsItemBoxUnlocked())
	{
		DetectorType type = pDetector->GetType();
		switch(type)
		{
		case DT_WANTER:
			sprintf(sStrTempDetect, "『 %s查看通缉令！<@", pCharacter->GetName());
			pUITextbox->SetText(sStrTempDetect);
			pUITextbox->SetIntoWanterScene();
			break;
		case DT_SELLER:
			pUITextbox->SetText("『 有计算机！<@");
			break;
		case DT_COMPUTER:
			pUITextbox->SetText("『 有自动售货机！<@");
			break;
		case DT_GOLD:
			{
				int randGoldGet = rand()%50 + 20;
				sprintf(sStrTempDetect, "『 %s打开箱子。<@『 找到了 %dG！<@", pCharacter->GetName(), randGoldGet);
				pUITextbox->SetText(sStrTempDetect);
				gGameStatusMgr.SetGoldNum(gGameStatusMgr.GetGoldNum() + randGoldGet);
				pDetector->SetItemBoxUnlock();
				FmodSound::PlaySnd(SND_GETITEM);
			}
			break;
		case DT_ITEM:
			if(gItemMgr.IsItemFull())
				strcpy_s(sStrTempDetect, "『 你拿的东西<    满满的！<@");
			else
			{
				sprintf(sStrTempDetect, "『 %s打开箱子。<@『 找到了 参丸！<@", pCharacter->GetName());
				gItemMgr.AddItem(ITEM_HP);
				pDetector->SetItemBoxUnlock();
				FmodSound::PlaySnd(SND_GETITEM);
			}
			pUITextbox->SetText(sStrTempDetect);
			break;
		case DT_EQUIP:
			if(gEquipMgr.IsEquipFull())
				strcpy_s(sStrTempDetect, "『 你的装备<    满满的！<@");
			else
			{
				sprintf(sStrTempDetect, "『 %s打开箱子。<@『 找到了 短枪！<@", pCharacter->GetName());
				gEquipMgr.AddEquip(EQUIP_SHORTGUN);
				pDetector->SetItemBoxUnlock();
				FmodSound::PlaySnd(SND_GETITEM);
			}
			pUITextbox->SetText(sStrTempDetect);
			break;
		case DT_CAVE:
			pUITextbox->SetText("『 发现山洞！<@『 布告上的水怪<    就躲在这里吧。<@");
			break;
		}
	}
	else
	{
		sprintf(sStrTempDetect, "『 %s检查脚下<@『 但什么都没发现。<@", pCharacter->GetName());
		pUITextbox->SetText(sStrTempDetect);
	}
}