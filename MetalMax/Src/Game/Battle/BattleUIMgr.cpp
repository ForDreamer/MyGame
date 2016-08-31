#include "BattleUIMgr.h"
#include "../../Main/Global.h"
#include "BatUIHumanMain.h"
#include "BatUITankMain.h"
#include "BatUIAtkList.h"
#include "BatUIAssistList.h"
#include "BatUIToolList.h"
#include "BatUITextBox.h"
#include "BatUIInfoBox.h"

DECLARE_SINGLETON(BattleUIMgr);
static BattleUIMgr sBattleUIMgr;

BattleUIMgr::BattleUIMgr(void)
{
	for(int i=0; i<BATUI_MAX; i++)
	{
		mpUIelems[i] = NULL;
	}
}
	
BattleUIMgr::~BattleUIMgr(void)
{
}

void BattleUIMgr::InitUIElements(void)
{
	for(int i=0; i<BATUI_MAX; i++)
	{
		switch(i)
		{
			case BATUI_HUMAN_MAIN: mpUIelems[i] = new BatUIHumanMain(); break;
			case BATUI_TANK_MAIN: mpUIelems[i] = new BatUITankMain(); break;
			case BATUI_ATK_LIST: mpUIelems[i] = new BatUIAtkList(); break;
			case BATUI_ASSIST_LIST: mpUIelems[i] = new BatUIAssistList(); break;
			case BATUI_TOOL_LIST: mpUIelems[i] = new BatUIToolList(); break;
			case BATUI_TEXTBOX: mpUIelems[i] = new BatUITextBox(); break;
			case BATUI_INFOBOX: mpUIelems[i] = new BatUIInfoBox(); break;
		}
		mpUIelems[i]->Init();
	}
}

void BattleUIMgr::ReleaseUIElements(void)
{
	for(int i=0; i<BATUI_MAX; i++)
	{
		delete mpUIelems[i];
		mpUIelems[i] = NULL;
	}
}

void BattleUIMgr::CloseAllUI(void)
{
	for(int i=0; i<BATUI_MAX; i++)
	{
		mpUIelems[i]->CloseUI();
	}
}

void BattleUIMgr::Update(void)
{
	for(int i=0; i<BATUI_MAX; i++)
	{
		if(mpUIelems[i]->IsVisible())
		{
			mpUIelems[i]->Update(0.0f, 0.0f);
		}
	}
}

void BattleUIMgr::Render(void)
{
	for(int i=0; i<BATUI_MAX; i++)
	{
		if(mpUIelems[i]->IsVisible())
		{
			mpUIelems[i]->Render();
		}
	}
}