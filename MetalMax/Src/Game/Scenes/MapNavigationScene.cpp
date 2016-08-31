#include "MapNavigationScene.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/Manager/RenderManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Main/Global.h"
#include "../MapEngine/MapManager.h"
#include "../MapEngine/MapCamera.h"
#include "../UI/UIManager.h"
#include "../Manager/GameStatusMgr.h"

bool MapNavigationScene::sbReadyToBattle = false;
float MapNavigationScene::sReadyTime = 0.0f;

MapNavigationScene::MapNavigationScene(SceneID id)
:	Scene(id),
	mBlinkTime(0.0f)
{
}

MapNavigationScene::~MapNavigationScene(void)
{
}

bool MapNavigationScene::Load(void)
{
	gUIMgr.CloseAllUI();
	gMapMgr.InitMap();
	gMapMgr.SetFreezed(false);
	gMapMgr.ClearMoveStep();

	//To Do: interactive npc/mapnode saveload
	if(gGameStatusMgr.GetGameState() >= GS_WIN_BOSS)
	{
		if(gMapMgr.GetCurMapID() == MAPID_CAVE_3)
		{
			gMapMgr.RemoveMapActor(gMapMgr.GetActor(41));
			gMapMgr.ReInitMapNode(31, 28, 43, true, true);
			gMapMgr.ReInitMapNode(31, 29, 43, true, true);
			gMapMgr.ReInitMapNode(31, 30, 43, true, true);
		}
	}
	if(gGameStatusMgr.GetGameState() >= GS_END_GAME)
	{
		if(gMapMgr.GetCurMapID() == MAPID_WORLD)
		{
			gMapMgr.ReInitMapNode(35, 18, 106, true, true);
		}
	}

	sbReadyToBattle = false;
	sReadyTime = 0.0f;
	mBlinkTime = 0.0f;
	
	mSprBlinking.SetSize(640.0f, 480.0f);
	mSprBlinking.SetPosition(0.0f, 0.0f);
	mSprBlinking.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	mSprBlinking.SetColor(1.0f, 1.0f, 1.0f, 0.75f);
	mSprBlinking.SetVisible(false);
	gRenderMgr.AddObject(&mSprBlinking);

	return true;
}

void MapNavigationScene::Release(void)
{
	gMapMgr.ReleaseMap();
	gRenderMgr.ClearAll();
	gUIMgr.CloseAllUI();
}

void MapNavigationScene::GotoNextScene(void)
{
	gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
}

void MapNavigationScene::Update(void)
{
	if(Global::KeyDown(KEY_A))
	{
		MapActor* pPlayer = gMapMgr.GetActor(PLAYER_ID);
		if(!gUIMgr.IsActive() && !pPlayer->IsMoving() && !sbReadyToBattle)
		{
			UIBase* pUI = gUIMgr.GetUI(UIT_MAIN);
			pUI->OpenUI();
			gUIMgr.SetActive(true);
			Global::ClearKeyState();

			FmodSound::PlaySnd(SND_CONFIRM);
		}
	}

	if(gMapMgr.GetCurMapID() == MAPID_WORLD)
	{
		MapActor* pPlayer = gMapMgr.GetActor(PLAYER_ID);
		if(pPlayer->GetTileX() == 36 && pPlayer->GetTileY() == 18)
		{
			gSceneMgr.SetNextScene(SCENE_ENDING);
		}
	}

	if(gMapMgr.GetCurMapID() == MAPID_WORLD || gMapMgr.GetCurMapID() == MAPID_CAVE_1
		|| gMapMgr.GetCurMapID() == MAPID_CAVE_2 || gMapMgr.GetCurMapID() == MAPID_CAVE_3)
	{
		if(!sbReadyToBattle)
		{
			if(gMapMgr.IsStepNotice())
			{
				gMapMgr.ClearStepNotice();
				int curStep = gMapMgr.GetMoveStep();
				int ran = rand()%25;
				if(curStep >= 15)
				{
					int check = (curStep - 15) + ran;
					if(check > 35)
					{
						StartBattle();
					}
				}
			}
		}
		else
		{
			MapCamera& camera = gMapMgr.GetCamera();
			float x = camera.GetPosX();
			float y = camera.GetPosY();
			mSprBlinking.SetPosition(x, y);

			if(sReadyTime < 1.0f)
			{
				mBlinkTime += Global::GetTimeDelta();
				if(mBlinkTime > 0.02f)
				{
					mBlinkTime = 0.0f;
					mSprBlinking.SetVisible(!mSprBlinking.GetVisible());
				}
			}
			else	
			{
				mSprBlinking.SetVisible(false);
			}

			sReadyTime += Global::GetTimeDelta();
			if(sReadyTime > 1.375f)
			{
				MapActor* pPlayer = gMapMgr.GetActor(PLAYER_ID);
				gMapMgr.SetNextMapInfo(gMapMgr.GetCurMapID(), pPlayer->GetTileX(), pPlayer->GetTileY(), pPlayer->GetCurDir());
				if(gMapMgr.IsPlayerInTank())
					gMapMgr.SetTankMapInfo(gMapMgr.GetCurMapID(), pPlayer->GetTileX(), pPlayer->GetTileY(), pPlayer->GetCurDir());
				
				gSceneMgr.SetNextScene(SCENE_BATTLE);
				sbReadyToBattle = false;
			}
		}
	}
}

void MapNavigationScene::StartBattle(void)
{
	gMapMgr.ClearMoveStep();
	gMapMgr.SetFreezed(true);
	gUIMgr.SetActive(false);

	sbReadyToBattle = true;
	sReadyTime = 0.0f;
	FmodSound::StopMusic();
	FmodSound::PlaySnd(SND_STARTBATTLE);
}
