#include "RenderManager.h"
#include "../../Main/Global.h"
#include "../Scene/Scene.h"
#include "../../Game/MapEngine/MapManager.h"
#include "../../Game/UI/UIManager.h"
#include "../../Game/Battle/BattleUIMgr.h"
#include "../../Game/Battle/BattleManager.h"
#include <assert.h>

DECLARE_SINGLETON(RenderManager);
static RenderManager sRenderManager;

RenderManager::RenderManager(void)
{
}
	
RenderManager::~RenderManager(void)
{
}

// To Do: refactor
void RenderManager::Process(void)
{
	if(Global::GetDevice())
	{
		Global::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
		Global::GetDevice()->BeginScene();

		gSceneMgr.GetFader()->Update();
		if(gMapMgr.IsActive())
			gMapMgr.Update();
		if(gBattleMgr.IsActive())
			gBattleMgr.Update();
		mHudObjs.Update();
		gUIMgr.Update();
		gBattleUIMgr.Update();
		
		if(gMapMgr.IsActive())
			gMapMgr.Render();
		if(gBattleMgr.IsActive())
			gBattleMgr.Render();
		mHudObjs.Render();
		gUIMgr.Render();
		gBattleUIMgr.Render();
		gSceneMgr.GetFader()->Render();
		
		Global::GetDevice()->EndScene();
		Global::GetDevice()->Present(0, 0, 0, 0);
	}
}

