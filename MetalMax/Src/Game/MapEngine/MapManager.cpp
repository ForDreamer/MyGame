#include "MapManager.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../UI/UIManager.h"

DECLARE_SINGLETON(MapManager);
static MapManager sMapManager;


void MapNodeLine::Init(int i)
{
	int curMapID = gMapMgr.GetCurMapID();
	int appendTex = sMapInfos[curMapID].append;
	int mapWidth = gMapMgr.GetMapWidth();
	int mapHeight = gMapMgr.GetMapHeight();
	int texID = 0;

	for(int k=0; k<mapWidth; k++)
	{
		MapNode* pNode = new MapNode();
		pNode->SetPos((k-mapWidth/2)*36.0f, (mapHeight/2-i)*30.0f);
		if(k < APPEND_WIDTH || k >= sMapInfos[curMapID].width + APPEND_WIDTH ||
			i < APPEND_HEIGHT || i >= sMapInfos[curMapID].height + APPEND_HEIGHT)
			texID = appendTex;
		else
			texID = gMapMgr.GetMapData(pNode->GetTileX()-APPEND_WIDTH, pNode->GetTileY()-APPEND_HEIGHT);

		pNode->Init(texID, sMapTexInfos[texID].actorMovable, sMapTexInfos[texID].tankMovable);
		Insert(pNode);
	}
}

void MapNodeLine::Release(void)
{
	for(unsigned int k=0; k<mMapNodeLine.size(); k++)
	{
		delete mMapNodeLine[k];
	}
	mMapNodeLine.clear();
}

void MapNodeLine::Update(void)
{
	for(unsigned int k=0; k<mMapNodeLine.size(); k++)
	{
		mMapNodeLine[k]->Update();
	}
}

void MapNodeLine::Render(void)
{
	for(unsigned int k=0; k<mMapNodeLine.size(); k++)
	{
		if(abs(gMapMgr.GetCamera().GetTileX() - mMapNodeLine[k]->GetTileX()) <= 9 
			&& abs(gMapMgr.GetCamera().GetTileY() - mMapNodeLine[k]->GetTileY()) <= 8)
		mMapNodeLine[k]->Render();
	}
}


void MapNodeArray::Init(void)
{
	for(int i=0; i<gMapMgr.GetMapHeight(); i++)
	{
		MapNodeLine* pLine = new MapNodeLine();
		pLine->Init(i);
		Insert(pLine);
	}
}

void MapNodeArray::Release(void)
{
	for(unsigned int i=0; i<mMapNodeArray.size(); i++)
	{
		mMapNodeArray[i]->Release();
		delete mMapNodeArray[i];
	}
	mMapNodeArray.clear();
}

void MapNodeArray::Update(void)
{
	for(unsigned int i=0; i<mMapNodeArray.size(); i++)
	{
		mMapNodeArray[i]->Update();
	}
}

void MapNodeArray::Render(void)
{
	for(unsigned int i=0; i<mMapNodeArray.size(); i++)
	{
		mMapNodeArray[i]->Render();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
MapManager::MapManager(void)
:	mMapWidth(0),
	mMapHeight(0),
	mCurMapId(MAPID_TOWN_MAIN),
	mNextMapId(MAPID_TOWN_MAIN),
	mNextTileX(23),
	mNextTileY(17),
	mNextDir(0),
	mTankMapId(MAPID_CAVE_3),
	mTankTileX(31),
	mTankTileY(32),
	mTankDir(3),
	mbActive(false),
	mpPlayer(NULL),
	mpTank(NULL),
	mbIsInTank(false),
	mMoveAccumStep(0),
	mbStepNotice(false),
	mbFreezed(false)
{
	ReleaseMapNodes();
	ReleaseMapRenderObjs();
	ReleaseMapTriggers();
	ReleaseMapActors();
	ReleaseMapDetectors();
}
	
MapManager::~MapManager(void)
{
}

void MapManager::Reset(void)
{
	mCurMapId = MAPID_TOWN_MAIN;
	mNextMapId = MAPID_TOWN_MAIN;
	mNextTileX = 23;
	mNextTileY = 17;
	mNextDir = 0;

	mTankMapId = MAPID_CAVE_3;
	mTankTileX = 31;
	mTankTileY = 32;
	mTankDir = 3;

	mbActive = false;
	mpPlayer = NULL;
	mpTank = NULL;
	mbIsInTank = false;

	mMoveAccumStep = 0;
	mbFreezed = false;
	mbStepNotice = false;
}

/************************************************************************/
/* ³õÊ¼»¯µØÍ¼                                                            */
/************************************************************************/
void MapManager::InitMap(void)
{
	assert(mNextMapId>=0 && mNextMapId<MAPID_MAX);
	assert(mNextTileX>=0 && mNextTileX<sMapInfos[mNextMapId].width + APPEND_WIDTH*2);
	assert(mNextTileY>=0 && mNextTileY<sMapInfos[mNextMapId].height + APPEND_HEIGHT*2);

	mCurMapId = mNextMapId;

	InitMapNodes();
	InitMapObjects();

	mCamera.SetPos(0, 0);
	mbActive = true;

	if(FmodSound::IsMusicStopped())
	{
		if(sMapInfos[mCurMapId].bgmIndex == BGM_WORLDTHEME && mbIsInTank)
			FmodSound::PlayMusic(BGM_TANKTHEME);
		else
			FmodSound::PlayMusic(sMapInfos[mCurMapId].bgmIndex);
	}
}

void MapManager::ReleaseMap(void)
{
	ReleaseMapNodes();
	ReleaseMapObjects();

	mCamera.SetPos(0, 0);
	mbActive = false;
}


void MapManager::InitMapNodes(void)
{
	mMapWidth = sMapInfos[mCurMapId].width + APPEND_WIDTH*2;
	mMapHeight = sMapInfos[mCurMapId].height + APPEND_HEIGHT*2;
	mpMapNodes.Init();
}

void MapManager::ReleaseMapNodes(void)
{
	mpMapNodes.Release();
}

void MapManager::InitMapObjects(void)
{
	for(int i=0; i<sMapInfos[mCurMapId].mapObjNum; i++)
	{
		const MapObjInfo& mapObjInfo = GetMapObjInfo(i);
		if(mapObjInfo.type == MOT_RENDER_OBJ)
		{
			InitMapRenderObjs(mapObjInfo);
		}
		else if(mapObjInfo.type == MOT_TRIGGER)
		{
			InitMapTriggers(mapObjInfo);
		}
		else if(mapObjInfo.type == MOT_ACTOR)
		{
			InitMapActors(mapObjInfo);
		}
		else if(mapObjInfo.type == MOT_DETECTOR)
		{
			InitMapDetectors(mapObjInfo);
		}
	}
	
	if(mTankMapId == mCurMapId)
	{
		MapNode* pMapNode = mpMapNodes.GetMapNode(mTankTileX, mTankTileY);
		MapActor* pTank = new MapActor();
		pTank->Init(TANK_ID, TEX_ACT_TANK_WALK_L, MapActor::AI_STAND, mTankDir, false);
		pTank->SetPos(pMapNode->GetPosX(), pMapNode->GetPosY());
		mMapActors.push_back(pTank);
	}
	MapNode* pMapNode = mpMapNodes.GetMapNode(mNextTileX, mNextTileY);
	MapActor* pPlayer = new MapActor();
	pPlayer->Init(PLAYER_ID, TEX_ACT_PLAYER_WALK_L, MapActor::AI_NONE, mNextDir, true);
	pPlayer->SetPos(pMapNode->GetPosX(), pMapNode->GetPosY());
	mMapActors.push_back(pPlayer);
	
	mpPlayer = GetActor(PLAYER_ID);
	mpTank = GetActor(TANK_ID);

	if(mbIsInTank)
		PlayerGotoTank(true);
}

void MapManager::ReleaseMapObjects(void)
{
	ReleaseMapRenderObjs();
	ReleaseMapTriggers();
	ReleaseMapActors();
	ReleaseMapDetectors();
}


void MapManager::InitMapRenderObjs(const MapObjInfo& mapObjInfo)
{
	MapNode* pMapNode = mpMapNodes.GetMapNode(mapObjInfo.tileX, mapObjInfo.tileY);
	assert(pMapNode);
	MapRenderObj* pRenderObj = new MapRenderObj();
	pRenderObj->Init((SystemTextureIndex)mapObjInfo.userdata1, mapObjInfo.userdata2);
	pRenderObj->SetPos(pMapNode->GetPosX()+mapObjInfo.userdata3, pMapNode->GetPosY()+mapObjInfo.userdata4);
	mMapRenderObjs.push_back(pRenderObj);
}

void MapManager::ReleaseMapRenderObjs(void)
{
	for(unsigned int i=0; i<mMapRenderObjs.size(); i++)
	{
		delete mMapRenderObjs[i];
	}
	mMapRenderObjs.clear();
}

void MapManager::InitMapTriggers(const MapObjInfo& mapObjInfo)
{
	MapNode* pMapNode = mpMapNodes.GetMapNode(mapObjInfo.tileX, mapObjInfo.tileY);
	assert(pMapNode);
	MapTrigger* pTrigger = new MapTrigger();
	EntryInfo entryInfo(mapObjInfo.userdata2, mapObjInfo.userdata3, mapObjInfo.userdata4);
	pTrigger->Init(mapObjInfo.userdata1, entryInfo);
	pTrigger->SetPos(pMapNode->GetPosX(), pMapNode->GetPosY());
	mMapTriggers.push_back(pTrigger);
}

void MapManager::ReleaseMapTriggers(void)
{
	for(unsigned int i=0; i<mMapTriggers.size(); i++)
	{
		delete mMapTriggers[i];
	}
	mMapTriggers.clear();
}

void MapManager::InitMapActors(const MapObjInfo& mapObjInfo)
{
	MapNode* pMapNode = mpMapNodes.GetMapNode(mapObjInfo.tileX, mapObjInfo.tileY);
	assert(pMapNode);
	MapActor* pActor = new MapActor();
	pActor->Init(mapObjInfo.userdata1, mapObjInfo.userdata2, mapObjInfo.userdata3, mapObjInfo.userdata4);
	pActor->SetPos(pMapNode->GetPosX(), pMapNode->GetPosY());
	mMapActors.push_back(pActor);
}

void MapManager::ReleaseMapActors(void)
{
	for(unsigned int i=0; i<mMapActors.size(); i++)
	{
		delete mMapActors[i];
	}
	mMapActors.clear();
}

void MapManager::InitMapDetectors(const MapObjInfo& mapObjInfo)
{
	MapNode* pMapNode = mpMapNodes.GetMapNode(mapObjInfo.tileX, mapObjInfo.tileY);
	assert(pMapNode);
	MapDetector* pDetector = new MapDetector();
	pDetector->Init(mapObjInfo.userdata1, mapObjInfo.userdata2);
	pDetector->SetPos(pMapNode->GetPosX(), pMapNode->GetPosY());
	mMapDetectors.push_back(pDetector);
}

void MapManager::ReleaseMapDetectors(void)
{
	for(unsigned int i=0; i<mMapDetectors.size(); i++)
	{
		delete mMapDetectors[i];
	}
	mMapDetectors.clear();
}


void MapManager::RemoveMapActor(MapActor* pActor)
{
	assert(pActor);
	vector<MapActor*>::iterator iter = find(mMapActors.begin(), mMapActors.end(), pActor);
	if (iter != mMapActors.end())
	{
		delete *iter;
		mMapActors.erase(iter);
	}
}

void MapManager::ReInitMapNode(int tileX, int tileY, int texId, bool actorMove, bool tankMove)
{
	MapNode* pNode = mpMapNodes.GetMapNode(tileX, tileY);
	assert(pNode);
	pNode->ReInit(texId, actorMove, tankMove);
}


void MapManager::Update(void)
{
	mpMapNodes.Update();
	if(!gUIMgr.IsActive() && !mbFreezed)
	{
		for(unsigned int i=0; i<mMapRenderObjs.size(); i++)
			mMapRenderObjs[i]->Update();
		for(unsigned int i=0; i<mMapActors.size(); i++)
			mMapActors[i]->Update();
		for(unsigned int i=0; i<mMapDetectors.size(); i++)
			mMapDetectors[i]->Update();

		UpdatePlayer();
		UpdateCamera();
	}
}

void MapManager::Render(void)
{
	mpMapNodes.Render();
	for(unsigned int i=0; i<mMapRenderObjs.size(); i++)
		mMapRenderObjs[i]->Render();
	for(unsigned int i=0; i<mMapActors.size(); i++)
		mMapActors[i]->Render();
	for(unsigned int i=0; i<mMapDetectors.size(); i++)
		mMapDetectors[i]->Render();
}

int MapManager::GetTileXFromPosX(int posX)
{
	int tileX = 0;
	if(posX >= 0)
		posX += 18;
	else if(posX < 0)
		posX -= 18;
	tileX = posX/36;
	tileX += mMapWidth/2;

	if(tileX < 0)
		tileX = 0;
	if(tileX > mMapWidth-1)
		tileX = mMapWidth-1;

	return tileX;
}

int MapManager::GetTileYFromPosY(int posY)
{
	int tileY = 0;
	if(posY >= 0)
		posY += 15;
	else if(posY < 0)
		posY -= 15;
	tileY = posY/30;
	tileY = mMapHeight/2 - tileY;

	if(tileY < 0)
		tileY = 0;
	if(tileY > mMapHeight-1)
		tileY = mMapHeight-1;

	return tileY;
}

int MapManager::GetMapData(int tileX, int tileY)
{
	assert(tileX < sMapInfos[mCurMapId].width);
	assert(tileY < sMapInfos[mCurMapId].height);

	// To Do: data driven
	int data = -1;
	switch(mCurMapId)
	{
		case MAPID_TOWN_MAIN: data = sMapData_TownMain[tileY][tileX]; break;
		case MAPID_TOWN_HERO: data = sMapData_TownHero[tileY][tileX]; break;
		case MAPID_TOWN_EQUIP: data = sMapData_TownEquip[tileY][tileX]; break;
		case MAPID_TOWN_BAR: data = sMapData_TownBar[tileY][tileX]; break;
		case MAPID_TOWN_INN_1: data = sMapData_TownInn1[tileY][tileX]; break;
		case MAPID_TOWN_INN_2: data = sMapData_TownInn2[tileY][tileX]; break;
		case MAPID_TOWN_TELE: data = sMapData_TownTele[tileY][tileX]; break;
		case MAPID_TOWN_TEC: data = sMapData_TownTec[tileY][tileX]; break;
		case MAPID_TOWN_HOME_1: data = sMapData_TownHome1[tileY][tileX]; break;
		case MAPID_TOWN_HOME_2: data = sMapData_TownHome2[tileY][tileX]; break;
		case MAPID_TOWN_HOME_3: data = sMapData_TownHome3[tileY][tileX]; break;
		case MAPID_WORLD: data = sMapData_World[tileY][tileX]; break;
		case MAPID_CAVE_1: data = sMapData_Cave1[tileY][tileX]; break;
		case MAPID_CAVE_2: data = sMapData_Cave2[tileY][tileX]; break;
		case MAPID_CAVE_3: data = sMapData_Cave3[tileY][tileX]; break;
		case MAPID_TOWN_GATE: data = sMapData_TownGate[tileY][tileX]; break;
	}

	assert(data >= 0);
	return data;
}

const MapObjInfo& MapManager::GetMapObjInfo(int index)
{
	assert(index < sMapInfos[mCurMapId].mapObjNum);

	// To Do: data driven
	switch(mCurMapId)
	{
		case MAPID_TOWN_MAIN: return sMapObjInfo_TownMain[index];
		case MAPID_TOWN_HERO: return sMapObjInfo_TownHero[index];
		case MAPID_TOWN_EQUIP: return sMapObjInfo_TownEquip[index];
		case MAPID_TOWN_BAR: return sMapObjInfo_TownBar[index];
		case MAPID_TOWN_INN_1: return sMapObjInfo_TownInn1[index];
		case MAPID_TOWN_INN_2: return sMapObjInfo_TownInn2[index];
		case MAPID_TOWN_TELE: return sMapObjInfo_TownTele[index];
		case MAPID_TOWN_TEC: return sMapObjInfo_TownTec[index];
		case MAPID_TOWN_HOME_1: return sMapObjInfo_TownHome1[index];
		case MAPID_TOWN_HOME_2: return sMapObjInfo_TownHome2[index];
		case MAPID_TOWN_HOME_3: return sMapObjInfo_TownHome3[index];
		case MAPID_WORLD: return sMapObjInfo_World[index];
		case MAPID_CAVE_1: return sMapObjInfo_Cave1[index];
		case MAPID_CAVE_2: return sMapObjInfo_Cave2[index];
		case MAPID_CAVE_3: return sMapObjInfo_Cave3[index];
		case MAPID_TOWN_GATE: return sMapObjInfo_TownGate[index];
	}
	assert(0);
}

void MapManager::UpdatePlayer(void)
{
	if(!mpPlayer->IsMoving() && !gSceneMgr.IsFading())
	{	
		int pTileX = mpPlayer->GetTileX();
		int pTileY = mpPlayer->GetTileY();
		if(Global::KeyPressed(KEY_LEFT))
		{
			if(pTileX-1 >= 0)
			{
				bool tankmove = true;
				if(mbIsInTank)
					tankmove = MoveActor(TANK_ID, pTileX-1, pTileY, MapActor::MOVE_DIR_LEFT);
				if(tankmove)
					MoveActor(PLAYER_ID, pTileX-1, pTileY, MapActor::MOVE_DIR_LEFT);
				else
					mpPlayer->SetMoving(MapActor::MOVE_DIR_LEFT, false);
			}
		}
		else if(Global::KeyPressed(KEY_UP))
		{
			if(pTileY-1 >= 0)
			{
				bool tankmove = true;
				if(mbIsInTank)
					tankmove = MoveActor(TANK_ID, pTileX, pTileY-1, MapActor::MOVE_DIR_UP);
				if(tankmove)
					MoveActor(PLAYER_ID, pTileX, pTileY-1, MapActor::MOVE_DIR_UP);
				else
					mpPlayer->SetMoving(MapActor::MOVE_DIR_UP, false);
			}
		}
		else if(Global::KeyPressed(KEY_RIGHT))
		{
			if(pTileX+1 < mMapWidth)
			{
				bool tankmove = true;
				if(mbIsInTank)
					tankmove = MoveActor(TANK_ID, pTileX+1, pTileY, MapActor::MOVE_DIR_RIGHT);
				if(tankmove)
					MoveActor(PLAYER_ID, pTileX+1, pTileY, MapActor::MOVE_DIR_RIGHT);
				else
					mpPlayer->SetMoving(MapActor::MOVE_DIR_RIGHT, false);
			}
		}
		else if(Global::KeyPressed(KEY_DOWN))
		{
			if(pTileY+1 < mMapHeight)
			{
				bool tankmove = true;
				if(mbIsInTank)
					tankmove = MoveActor(TANK_ID, pTileX, pTileY+1, MapActor::MOVE_DIR_DOWN);
				if(tankmove)
					MoveActor(PLAYER_ID, pTileX, pTileY+1, MapActor::MOVE_DIR_DOWN);
				else
					mpPlayer->SetMoving(MapActor::MOVE_DIR_DOWN, false);
			}
		}
	}
}

void MapManager::UpdateCamera(void)
{
	mCamera.SetPos(mpPlayer->GetPosX(), mpPlayer->GetPosY());
}


bool MapManager::MoveActor(int actorId, int targetTileX, int targetTileY, MapActor::MoveDir moveDir)
{
	MapActor* pActor = GetActor(actorId);
	if(!pActor)
	{
		assert(0);
		return false;
	}

	MapNode* pMapNode = mpMapNodes.GetMapNode(targetTileX, targetTileY);
	bool movable = pMapNode->IsActorMovable();
	if(mbIsInTank)
		movable = pMapNode->IsTankMovable();

	for(unsigned int i=0; i<mMapActors.size(); i++)
	{
		MapActor* pNearActor = mMapActors[i];
		if(pNearActor == pActor || pNearActor->GetActorID() == TANK_ID)
			continue;

		if(pNearActor->GetTileX() == targetTileX && pNearActor->GetTileY() == targetTileY)
		{
			movable = false;
			break;
		}
		else if(pNearActor->GetMoveTargetTileX() == targetTileX && pNearActor->GetMoveTargetTileY() == targetTileY)
		{
			movable = false;
			break;
		}
	}
	for(unsigned int i=0; i<mMapDetectors.size(); i++)
	{
		MapDetector* pDetector = mMapDetectors[i];
		if(pDetector->GetTileX() == targetTileX && pDetector->GetTileY() == targetTileY)
		{
			movable = false;
			break;
		}
	}

	if(actorId != PLAYER_ID && actorId != TANK_ID)
	{
		MapTrigger* pTrigger = GetTrigger(targetTileX, targetTileY);
		if(pTrigger)
			movable = false;
	}

	pActor->SetMoveTarget(pMapNode->GetPosX(), pMapNode->GetPosY());
	pActor->SetMoveTargetTile(targetTileX, targetTileY);
	pActor->SetMoving(moveDir, movable);

	if(actorId == PLAYER_ID)
	{
		MapTrigger* pTrigger = GetTrigger(targetTileX, targetTileY);
		if(pTrigger && pTrigger->GetType() == TYPE_ENTER_DOOR)
		{
			MapRenderObj* pRenderObj = GetRenderObj(pTrigger->GetEntryInfo().targetMapId);
			assert(pRenderObj);
			if(pRenderObj->IsVisible())
			{
				pRenderObj->SetVisible(false);
				FmodSound::PlaySnd(SND_OPENDOOR);
			}
		}
	}

	return movable;
}

bool MapManager::TestTriggers(MapActor* pActor)
{
	assert(pActor);
	if(pActor == mpPlayer)
	{
		for(unsigned int i=0; i<mMapTriggers.size(); i++)
		{
			MapTrigger* pTrigger = mMapTriggers[i];
			if(pTrigger->GetType() == TYPE_ENTRY)
			{
				if(pActor->GetTileX() == pTrigger->GetTileX()
					&& pActor->GetTileY() == pTrigger->GetTileY())
				{
					const EntryInfo& entryInfo= pTrigger->GetEntryInfo();
					mNextMapId = (MapID)entryInfo.targetMapId;
					mNextTileX = entryInfo.targetTileX;
					mNextTileY = entryInfo.targetTileY;
					mNextDir = pActor->GetCurDir();

					if(mbIsInTank)
					{
						mTankMapId = mNextMapId;
						mTankTileX = mNextTileX;
						mTankTileY = mNextTileY;
						mTankDir = mNextDir;
					}

					gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
					if(mpMapNodes.GetMapNode(pTrigger->GetTileX(), pTrigger->GetTileY())->GetTexID() == MT_SKY_1)
						FmodSound::PlaySnd(SND_FALL);
					else
						FmodSound::PlaySnd(SND_INTOENTRY);
					if(sMapInfos[mCurMapId].bgmIndex != sMapInfos[mNextMapId].bgmIndex)
					{
						FmodSound::StopMusic();
					}
					return true;
				}
			}
			else if(pTrigger->GetType() == TYPE_LEFT_DOOR)
			{
				if(pActor->GetTileX() == pTrigger->GetTileX()
					&& pActor->GetTileY() == pTrigger->GetTileY())
				{
					MapRenderObj* pRenderObj = GetRenderObj(pTrigger->GetEntryInfo().targetMapId);
					assert(pRenderObj);
					if(!pRenderObj->IsVisible())
					{
						pRenderObj->SetVisible(true);
						FmodSound::PlaySnd(SND_OPENDOOR);
					}
				}
			}
		}
	}
	return false;
}

MapRenderObj* MapManager::GetRenderObj(int texId)
{
	int pTileX = mpPlayer->GetTileX();
	int pTileY = mpPlayer->GetTileY();

	MapRenderObj* pRenderObj = NULL;
	for(unsigned int i=0; i<mMapRenderObjs.size(); i++)
	{
		if(mMapRenderObjs[i]->GetTexID() == texId
			&& abs(pTileX - mMapRenderObjs[i]->GetTileX() <= 1)
			&& abs(pTileY - mMapRenderObjs[i]->GetTileY() <= 1))
		{
			pRenderObj = mMapRenderObjs[i];
			break;
		}
	}
	return pRenderObj;
}

MapTrigger* MapManager::GetTrigger(int tileX, int tileY)
{
	MapTrigger* pTrigger = NULL;
	for(unsigned int i=0; i<mMapTriggers.size(); i++)
	{
		if(mMapTriggers[i]->GetTileX() == tileX && mMapTriggers[i]->GetTileY() == tileY)
		{
			pTrigger = mMapTriggers[i];
			break;
		}
	}
	return pTrigger;
}

MapDetector* MapManager::GetDetector(int tileX, int tileY)
{
	MapDetector* pDetector = NULL;
	for(unsigned int i=0; i<mMapDetectors.size(); i++)
	{
		if(mMapDetectors[i]->GetTileX() == tileX && mMapDetectors[i]->GetTileY() == tileY)
		{
			pDetector = mMapDetectors[i];
			break;
		}
	}
	return pDetector;
}

MapActor* MapManager::GetActor(int tileX, int tileY)
{
	MapActor* pActor = NULL;
	for(unsigned int i=0; i<mMapActors.size(); i++)
	{
		if(mMapActors[i]->GetTileX() == tileX && mMapActors[i]->GetTileY() == tileY)
		{
			pActor = mMapActors[i];
			break;
		}
	}
	return pActor;
}

MapActor* MapManager::GetActor(int actorId)
{
	MapActor* pActor = NULL;
	for(unsigned int i=0; i<mMapActors.size(); i++)
	{
		if(mMapActors[i]->GetActorID() == actorId)
		{
			pActor = mMapActors[i];
			break;
		}
	}
	return pActor;
}

int MapManager::GetTargetActorId(void)
{
	int tileX = mpPlayer->GetTileX();
	int tileY = mpPlayer->GetTileY();
	int dir = mpPlayer->GetCurDir();

	if(dir == MapActor::MOVE_DIR_LEFT)
		tileX -= 1;
	else if(dir == MapActor::MOVE_DIR_UP)
		tileY -= 1;
	else if(dir == MapActor::MOVE_DIR_RIGHT)
		tileX += 1;
	else if(dir == MapActor::MOVE_DIR_DOWN)
		tileY += 1;

	MapActor* pActor = GetActor(tileX, tileY);
	if(pActor)
		return pActor->GetActorID();
	else
	{
		// To Do: refactor..
		int mapdata = GetMapData(tileX-APPEND_WIDTH, tileY-APPEND_HEIGHT);
		if(mapdata == MT_WALL_5
			|| mapdata == MT_WALL_19
			|| mapdata == MT_WALL_20
			|| mapdata == MT_APPEND_1
			|| mapdata == MT_FLOOR_12
			|| mapdata == MT_WALL_83)
		{
			if(dir == MapActor::MOVE_DIR_LEFT)
				tileX -= 1;
			else if(dir == MapActor::MOVE_DIR_UP)
				tileY -= 1;
			else if(dir == MapActor::MOVE_DIR_RIGHT)
				tileX += 1;
			else if(dir == MapActor::MOVE_DIR_DOWN)
				tileY += 1;

			MapActor* pActor = GetActor(tileX, tileY);
			if(pActor)
				return pActor->GetActorID();
		}
	}

	return -1;
}

MapActor::MoveDir MapManager::GetActorTalkingDir(MapActor* pActor)
{
	int resultDir = -1;
	int pTileX = mpPlayer->GetTileX();
	int pTileY = mpPlayer->GetTileY();
	int tTileX = pActor->GetTileX();
	int tTileY = pActor->GetTileY();

	if(pTileX < tTileX && pTileY == tTileY)
		resultDir = MapActor::MOVE_DIR_LEFT;
	if(pTileX > tTileX && pTileY == tTileY)
		resultDir = MapActor::MOVE_DIR_RIGHT;
	if(pTileY < tTileY && pTileX == tTileX)
		resultDir = MapActor::MOVE_DIR_UP;
	if(pTileY > tTileY && pTileX == tTileX)
		resultDir = MapActor::MOVE_DIR_DOWN;

	assert(resultDir >= 0);
	return MapActor::MoveDir(resultDir);
}

MapDetector* MapManager::GetTargetDetector(void)
{
	MapActor::MoveDir dir = mpPlayer->GetCurDir();
	int pTileX = mpPlayer->GetTileX();
	int pTileY = mpPlayer->GetTileY();
	if(dir == MapActor::MOVE_DIR_LEFT)
		pTileX -= 1;
	else if(dir == MapActor::MOVE_DIR_RIGHT)
		pTileX += 1;
	else if(dir == MapActor::MOVE_DIR_UP)
		pTileY -= 1;
	else if(dir == MapActor::MOVE_DIR_DOWN)
		pTileY += 1;

	return GetDetector(pTileX, pTileY);
}


bool MapManager::IsPlayerCanGotoTank(void)
{
	if(!mpTank)
		return false;

	if(mpPlayer->GetTileX() == mpTank->GetTileX()
		&& mpPlayer->GetTileY() == mpTank->GetTileY())
	{
		return true;
	}
	return false;
}

void MapManager::PlayerGotoTank(bool on)
{
	if(on)
	{
		mpPlayer->SetVisible(false);
		mpTank->SetIsPlayer(true);
		mbIsInTank = true;
	}
	else
	{
		mpPlayer->SetVisible(true);
		mpTank->SetIsPlayer(false);
		mbIsInTank = false;

		mTankMapId = mCurMapId;
		mTankTileX = mpPlayer->GetTileX();
		mTankTileY = mpPlayer->GetTileY();
		mTankDir = mpPlayer->GetCurDir();
	}
}