#include "UIManager.h"
#include "UIMain.h"
#include "UIMisc.h"
#include "UITop.h"
#include "../../Main/Global.h"
#include "../MapEngine/MapManager.h"

DECLARE_SINGLETON(UIManager);
static UIManager sUIManager;

UIManager::UIManager(void)
:	mbActive(false)
{
	for(int i=0; i<UIT_MAX; i++)
	{
		mpUIelems[i] = NULL;
	}
}
	
UIManager::~UIManager(void)
{
}
/************************************************************************/
/* 初始化UI界面元素														*/
/************************************************************************/
void UIManager::InitUIElements(void)
{
	for(int i=0; i<UIT_MAX; i++)
	{
		switch(i)
		{
			case UIT_MAIN: mpUIelems[i] = new UIMain(); break;
			case UIT_TEXTBOX: mpUIelems[i] = new UITextBox(); break;
			case UIT_SELECTOR: mpUIelems[i] = new UISelector(); break;
			case UIT_BATTLEINFO: mpUIelems[i] = new UIBattleInfo(); break;
			case UIT_TOOLS: mpUIelems[i] = new UITools(); break;
			case UIT_CANNON: mpUIelems[i] = new UICannon(); break;
			case UIT_EQUIP: mpUIelems[i] = new UIEquip(); break;
			case UIT_STATUS: mpUIelems[i] = new UIStatus(); break;
			case UIT_TOP: mpUIelems[i] = new UITop(); break;
			case UIT_SELL: mpUIelems[i] = new UISell(); break;
			default: assert(0);
		}
		mpUIelems[i]->Init();
	}
}

void UIManager::ReleaseUIElements(void)
{
	for(int i=0; i<UIT_MAX; i++)
	{
		delete mpUIelems[i];
		mpUIelems[i] = NULL;
	}
}

void UIManager::CloseAllUI(void)
{
	for(int i=0; i<UIT_MAX; i++)
	{
		mpUIelems[i]->CloseUI();
	}
	mbActive = false;
}

void UIManager::Update(void)
{
	MapCamera& camera = gMapMgr.GetCamera();
	float baseX = camera.GetPosX();
	float baseY = camera.GetPosY();

	for(int i=0; i<UIT_MAX; i++)
	{
		if(mpUIelems[i]->IsVisible())
		{
			mpUIelems[i]->Update(baseX, baseY);
		}
	}
}

void UIManager::Render(void)
{
	for(int i=0; i<UIT_MAX; i++)
	{
		if(mpUIelems[i]->IsVisible())
		{
			mpUIelems[i]->Render();
		}
	}
}