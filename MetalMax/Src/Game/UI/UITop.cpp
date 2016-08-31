#include "UITop.h"
#include "UIManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/ItemMgr.h"
#include "../Manager/EquipMgr.h"
#include "../Manager/GameStatusMgr.h"
#include "../MapEngine/MapManager.h"
#include "../Manager/BattleTankMgr.h"
#include <strsafe.h>

static const char* sStrDrinks[MAX_LINES] = {"ơ��", "�մ�ˮ", "", ""};
static const char* sStrBars[MAX_LINES] = {"ơ��                         3 G", "�մ�ˮ                     5 G", "", ""};
static const char* sStrHeros[MAX_LINES] = {"���鱨", "�콱��", "�˳�", ""};
static const char* sStrRooms[MAX_LINES] = {"÷", "��", "��", ""};
static const char* sStrInns[MAX_LINES] = {"÷                           10 G", "��                           30 G", "��                         150 G", ""};
static const char* sStrCannons[MAX_LINES] = {"��ҩ", "װ��Ƭ", "�˳�", ""};
static const char* sStrToolEquips[MAX_LINES] = {"��ʲô", "��ʲô", "�˳�", ""};
static const char* sStrToolItems[MAX_LINES] = {"����                         10 G", "����                           5 G", "��ƿ                         20 G", ""};
static const char* sStrEquipItems[MAX_LINES] = {"��ǹ                       120 G", "������                     15 G", "����                         40 G", "��Ь                         30 G"};
static const char* sStrToolNames[MAX_LINES] = {"����", "����", "��ƿ", ""};
static const char* sStrEquipNames[MAX_LINES] = {"��ǹ", "������", "����", "��Ь"};
static const int sToolListId[MAX_LINES] = {ITEM_HP, ITEM_BOMB, ITEM_FIRE, -1};
static const int sEquipListId[MAX_LINES] = {EQUIP_SHORTGUN, EQUIP_GLOVE, EQUIP_CLOTH, EQUIP_SHOE};

static char sStrTempInfo[64];
static char sStrGoldInfo[16];

UITop::UITop(void)
:	mType(UITOP_BAR),
	mMaxItemNum(0)
{
}

UITop::~UITop(void)
{
}

void UITop::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectId = 0;
	sprintf_s(sStrGoldInfo, "G: %d", gGameStatusMgr.GetGoldNum());
	mTxtGold.SetText(sStrGoldInfo);

	switch(mType)
	{
	case UITOP_BAR:
		mMaxItemNum = 2;
		mTxtPlace1.SetText("�ư�");
		mTxtPlace2.SetText("����");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrBars[i]);
		}
		break;
	case UITOP_HERO:
		mMaxItemNum = 3;
		mTxtPlace1.SetText("����");
		mTxtPlace2.SetText("�̵�");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrHeros[i]);
		}
		break;
	case UITOP_INN:
		mMaxItemNum = 3;
		mTxtPlace1.SetText("�ù�");
		mTxtPlace2.SetText("ס��");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrInns[i]);
		}
		break;
	case UITOP_CANNON:
		mMaxItemNum = 3;
		mTxtPlace1.SetText("ս��");
		mTxtPlace2.SetText("����");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrCannons[i]);
		}
		break;
	case UITOP_TOOL:
		mMaxItemNum = 3;
		mTxtPlace1.SetText("����");
		mTxtPlace2.SetText("����");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrToolEquips[i]);
		}
		break;
	case UITOP_EQUIP:
		mMaxItemNum = 3;
		mTxtPlace1.SetText("����");
		mTxtPlace2.SetText("װ��");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrToolEquips[i]);
		}
		break;
	case UITOP_TOOL_ITEM:
		mMaxItemNum = 3;
		mTxtPlace1.SetText("����");
		mTxtPlace2.SetText("����");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrToolItems[i]);
		}
		break;
	case UITOP_EQUIP_ITEM:
		mMaxItemNum = 4;
		mTxtPlace1.SetText("����");
		mTxtPlace2.SetText("װ��");
		for(int i=0; i<MAX_LINES; i++)
		{
			mTxtLines[i].SetText(sStrEquipItems[i]);
		}
		break;
	}
}

void UITop::CloseUI(void)
{
	UIBase::CloseUI();
}

void UITop::Init(void)
{
	mSprBG.SetSize(642.0f, 256.0f);
	mSprBG.SetTexture(gTexMgr.GetUITex(TEX_UI_TOP));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mSprCursor.SetSize(36.0f, 30.0f);
	mSprCursor.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mSprCursor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	mTxtPlace1.SetFontType(FT_BIGFONT);
	mTxtPlace1.SetPosition(-240.0f, 200.0f);
	mTxtPlace2.SetFontType(FT_BIGFONT);
	mTxtPlace2.SetPosition(-240.0f, 170.0f);
	mTxtGold.SetPosition(-250.0f, 100.0f);
	for(int i=0; i<MAX_LINES; i++)
	{
		mTxtLines[i].SetFontType(FT_BIGFONT);
		mTxtLines[i].SetPosition(-40.0f, 215.0f - i*32.0f);
	}
}

void UITop::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_B))
		{
			if(mType == UITOP_TOOL_ITEM || mType == UITOP_EQUIP_ITEM)
			{
				UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
				sprintf_s(sStrTempInfo, "<@");
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
				pUITextbox->SetOnFocus(false);

				SetUIType((UIT_Type)(mType-2));
				OpenUI();
			}
			else
			{
				CloseUI();
				gUIMgr.CloseAllUI();
			}

			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_A))
		{
			SetOnFocus(false);
			OnItemSelected();
			
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

	mSprBG.SetPosition(baseX, baseY+153.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX-60.0f, baseY+200.0f - mSelectId*32.0f);
	mSprCursor.Update();

	mTxtPlace1.Update();
	mTxtPlace2.Update();
	mTxtGold.Update();
	for(int i=0; i<mMaxItemNum; i++)
	{
		mTxtLines[i].Update();
	}
}

void UITop::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();

	mTxtPlace1.Render();
	mTxtPlace2.Render();
	mTxtGold.Render();
	for(int i=0; i<mMaxItemNum; i++)
	{
		mTxtLines[i].Render();
	}
}

void UITop::OnItemSelected(void)
{
	UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
	UISell* pUISell = (UISell*)(gUIMgr.GetUI(UIT_SELL));

	switch(mType)
	{
	case UITOP_BAR:
		{
			int gold = gGameStatusMgr.GetGoldNum();
			int cost;
			if(mSelectId == 0)
				cost = 3;
			else if(mSelectId == 1)
				cost = 5;

			if(gold < cost)
			{
				sprintf_s(sStrTempInfo, "�� Ǯ������,<    �����ɣ�<@");	
			}
			else
			{
				Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
				sprintf_s(sStrTempInfo, "�� %s����%s��<@�� �����Щ�ˡ�<@", pPlayer->GetName(), sStrDrinks[mSelectId]);
				gold -= cost;
				gGameStatusMgr.SetGoldNum(gold);
			}
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
		}
		break;
	case UITOP_HERO:
		if(mSelectId == 0)
			sprintf_s(sStrTempInfo, "�� �����ϵ�ˮ�֣�<  ����ɽ���<@");
		else if(mSelectId == 1)
			sprintf_s(sStrTempInfo, "�� ����ǽ�ϵ�ͨ���<@�� �Ƕ�д�ţ�<  ��ܹ�����������͡�<@");
		else if(mSelectId == 2)
			sprintf_s(sStrTempInfo, "�� ����������<@");
		pUITextbox->CloseUI();
		pUITextbox->OpenUI();
		pUITextbox->SetText(sStrTempInfo);
		break;
	case UITOP_INN:
		{
			pUITextbox->OpenUI();
			int gold = gGameStatusMgr.GetGoldNum();
			int cost;
			if(mSelectId == 0)
				cost = 10;
			else if(mSelectId == 1)
				cost = 30;
			else if(mSelectId == 2)
				cost = 150;

			if(gold < cost)
			{
				sprintf_s(sStrTempInfo, "�� Ǯ������,<    �����ɣ�<@");
				pUITextbox->SetText(sStrTempInfo);
			}
			else
			{
				sprintf_s(sStrTempInfo, "�� ��%sѽ��<  ��������<@�� ����<@", sStrRooms[mSelectId]);
				pUITextbox->SetText(sStrTempInfo);

				if(mSelectId == 0)
				{
					gMapMgr.SetNextMapInfo(MAPID_TOWN_INN_1, 12, 13, 0);
				}
				if(mSelectId == 1)
				{
					gMapMgr.SetNextMapInfo(MAPID_TOWN_INN_1, 18, 13, 0);
				}
				if(mSelectId == 2)
				{
					gMapMgr.SetNextMapInfo(MAPID_TOWN_INN_2, 15, 20, 0);
				}
				gold -= cost;
				gGameStatusMgr.SetGoldNum(gold);
				pUITextbox->SetIntoInnScene();
			}
		}
		break;
	case UITOP_CANNON:
		{
			BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
			if(mSelectId == 0)
			{
				if(gMapMgr.IsPlayerInTank())
				{
					sprintf_s(sStrTempInfo, "�� ��ҩȫ��װ����<  �����ϣ�<@");
					pTank->SetST(pTank->GetSTMax());
				}
				else
					sprintf_s(sStrTempInfo, "�� û��ս��<  û�����䣡<@");
			}
			else if(mSelectId == 1)
			{
				if(gMapMgr.IsPlayerInTank())
				{
					sprintf_s(sStrTempInfo, "�� װ��Ƭ������ϣ�<@");
					pTank->SetSP(pTank->GetSPMax());
				}
				else
					sprintf_s(sStrTempInfo, "�� û��ս��<  û�����䣡<@");
			}
			else if(mSelectId == 2)
				sprintf_s(sStrTempInfo, "�� ��������<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
		}
		break;
	case UITOP_TOOL:
		if(mSelectId == 0)
		{
			sprintf_s(sStrTempInfo, "�� ���ĸ�?<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
			pUITextbox->SetOnFocus(false);

			SetUIType(UITOP_TOOL_ITEM);
			OpenUI();
		}
		else if(mSelectId == 1)
		{
			sprintf_s(sStrTempInfo, "�� ���ĸ�?<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
			pUITextbox->SetOnFocus(false);

			pUISell->SetUISType(UIS_TOOL);
			pUISell->OpenUI();
		}
		else if(mSelectId == 2)
		{
			sprintf_s(sStrTempInfo, "�� ��������<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
		}
		break;
	case UITOP_EQUIP:
		if(mSelectId == 0)
		{		
			sprintf_s(sStrTempInfo, "�� ���ĸ�?<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
			pUITextbox->SetOnFocus(false);

			SetUIType(UITOP_EQUIP_ITEM);
			OpenUI();
		}
		else if(mSelectId == 1)
		{
			sprintf_s(sStrTempInfo, "�� ���ĸ�?<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
			pUITextbox->SetOnFocus(false);

			pUISell->SetUISType(UIS_EQUIP);
			pUISell->OpenUI();
		}
		else if(mSelectId == 2)
		{
			sprintf_s(sStrTempInfo, "�� ��������<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
		}
		break;
	case UITOP_TOOL_ITEM:
		{
			int itemNum = gItemMgr.GetItemNum();
			int goldNum = gGameStatusMgr.GetGoldNum();
			ItemID itemId = (ItemID)(sToolListId[mSelectId]);

			if(itemNum >= MAX_ITEM_NUM)
			{
				strcpy_s(sStrTempInfo, "�� ���õĶ���<    �����ģ�<@");
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
			}
			else if(goldNum < sItemCostList[itemId])
			{
				strcpy_s(sStrTempInfo, "�� ���ź�<    Ǯ��������<@");
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
			}
			else
			{
				Item* pItem = gItemMgr.AddItem(itemId);
				sprintf_s(sStrTempInfo, "�� ��%s��лл��<@  ����%d G��<@", pItem->GetName(), pItem->GetCost());
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
				goldNum -= pItem->GetCost();
				gGameStatusMgr.SetGoldNum(goldNum);
			}
		}
		break;
	case UITOP_EQUIP_ITEM:
		{
			int equipNum = gEquipMgr.GetEquipNum();
			int goldNum = gGameStatusMgr.GetGoldNum();
			EquipID equipId = (EquipID)(sEquipListId[mSelectId]);

			if(equipNum >= MAX_EQUIP_NUM)
			{
				strcpy_s(sStrTempInfo, "�� ���װ��<    �����ģ�<@");
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
			}
			else if(goldNum < sEquipCostList[equipId])
			{
				strcpy_s(sStrTempInfo, "�� ���ź�<    Ǯ��������<@");
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
			}
			else
			{
				Equip* pEquip = gEquipMgr.AddEquip(equipId);
				sprintf_s(sStrTempInfo, "�� ��%s��лл��<@  ����%d G��<@", pEquip->GetName(), pEquip->GetCost());
				pUITextbox->OpenUI();
				pUITextbox->SetText(sStrTempInfo);
				goldNum -= pEquip->GetCost();
				gGameStatusMgr.SetGoldNum(goldNum);
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
static char sTempSellName[8];

UISell::UISell(void)
:	mType(UIS_TOOL),
	mSelectRow(0),
	mSelectCol(0),
	mMaxItemNum(0)
{
}

UISell::~UISell(void)
{
}

void UISell::OpenUI(void)
{
	UIBase::OpenUI();

	mSelectRow = 0;
	mSelectCol = 0;
	mSelectId = 0;

	if(mType == UIS_TOOL)
		mMaxItemNum = gItemMgr.GetItemNum();
	else
		mMaxItemNum = 7;

	UpdateSellText();
}

void UISell::CloseUI(void)
{
	UIBase::CloseUI();
}

void UISell::UpdateSellText(void)
{
	for(int i=0; i<MAX_ITEMS; i++)
	{
		if(mType == UIS_TOOL)
		{
			Item* pItem = gItemMgr.GetItem(i);
			if(pItem)
				mTxtLines[i].SetText(pItem->GetName());
			else
				mTxtLines[i].SetText("");
		}
		else if(mType == UIS_EQUIP)
		{
			Equip* pEquip = gEquipMgr.GetEquip(i);
			if(pEquip)
				mTxtLines[i].SetText(pEquip->GetName());
			else
				mTxtLines[i].SetText("");
		}
	}
}

void UISell::Init(void)
{
	mSprBG.SetSize(642.0f, 256.0f);
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
			mTxtLines[i].SetPosition(-20.0f, 215.0f - yIndex*32.0f);
		}
		else
		{
			mTxtLines[i].SetPosition(140.0f, 215.0f - yIndex*32.0f);
			yIndex += 1.0f;
		}
	}
}

void UISell::Update(float baseX, float baseY)
{
	if(IsOnFocus())
	{
		if(Global::KeyDown(KEY_B))
		{
			CloseUI();

			UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));
			sprintf_s(sStrTempInfo, "<@");
			pUITextbox->OpenUI();
			pUITextbox->SetText(sStrTempInfo);
			pUITextbox->SetOnFocus(false);

			UITop* pUITop = (UITop*)(gUIMgr.GetUI(UIT_TOP));
			pUITop->SetOnFocus(true);


			Global::ClearKeyState();
			FmodSound::PlaySnd(SND_CONFIRM);
		}

		if(Global::KeyDown(KEY_A))
		{
			SetOnFocus(false);
			OnItemSelected();
			
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

	mSprBG.SetPosition(baseX+100.0f, baseY+153.0f);
	mSprBG.Update();
	mSprCursor.SetPosition(baseX-45.0f + 160.0f*mSelectCol, baseY+200.0f - 32.0f*mSelectRow);
	mSprCursor.Update();
	for(int i=0; i<mMaxItemNum; i++)
	{
		mTxtLines[i].Update();
	}
}

void UISell::Render(void)
{
	mSprBG.Render();
	mSprCursor.Render();
	for(int i=0; i<mMaxItemNum; i++)
	{
		mTxtLines[i].Render();
	}
}

void UISell::OnItemSelected(void)
{
	UITextBox* pUITextbox = (UITextBox*)(gUIMgr.GetUI(UIT_TEXTBOX));

	if(mType == UIS_TOOL)
	{
		Item* pItem = gItemMgr.GetItem(mSelectId);
		assert(pItem);
		int cost = (int)(pItem->GetCost()*0.75f);
		if(cost == 0)
		{
			strcpy_s(sStrTempInfo, "�� �Ǹ�������������<@");
		}
		else
		{
			sprintf_s(sStrTempInfo, "�� ��%s���ð���<@  ����%d G��<@", pItem->GetName(), cost);

			strcpy_s(sTempSellName, pItem->GetName());
			mTxtLines[mSelectId].SetText(sTempSellName);
			gItemMgr.RemoveItem(mSelectId);

			int gold = gGameStatusMgr.GetGoldNum();
			gold += cost;
			gGameStatusMgr.SetGoldNum(gold);
		}
	}
	else if(mType == UIS_EQUIP)
	{
		Equip* pEquip = gEquipMgr.GetEquip(mSelectId);
		assert(pEquip);
		int cost = (int)(pEquip->GetCost()*0.75f);
		if(cost == 0)
		{
			strcpy_s(sStrTempInfo, "�� �Ǹ�������������<@");
		}
		else
		{
			sprintf_s(sStrTempInfo, "�� ��%s���ð���<@  ����%d G��<@", pEquip->GetName(), cost);

			strcpy_s(sTempSellName, pEquip->GetName());
			mTxtLines[mSelectId].SetText(sTempSellName);
			pEquip->UnApplyEquip();
			gEquipMgr.RemoveEquip(mSelectId);

			int gold = gGameStatusMgr.GetGoldNum();
			gold += cost;
			gGameStatusMgr.SetGoldNum(gold);
		}
	}
	pUITextbox->OpenUI();
	pUITextbox->SetText(sStrTempInfo);
}