#ifndef _MapManager_H_
#define _MapManager_H_

#include "../../Engine/Misc/Singleton.h"
#include "MapTexDef.h"
#include "MapConfig.h"
#include "MapNode.h"
#include "MapRenderObj.h"
#include "MapTrigger.h"
#include "MapActor.h"
#include "MapCamera.h"
#include "MapDetector.h"
#include <vector>
#include <algorithm>

using namespace std;

#define PLAYER_ID 0
#define TANK_ID 42

class MapNodeLine
{
	typedef vector<MapNode*>::iterator LineIter;
public:
	MapNodeLine(void) { mMapNodeLine.clear(); }
	void Init(int i);
	void Release(void);
	void Update(void);
	void Render(void);
	void Insert(MapNode* pNode)
	{
		assert(pNode);
		mMapNodeLine.push_back(pNode);
	}
	MapNode* GetMapNode(int tileX) { return mMapNodeLine[tileX]; }

private:
	vector<MapNode*> mMapNodeLine;
};

class MapNodeArray
{
	typedef vector<MapNodeLine*>::iterator ArrayIter;
public:
	MapNodeArray(void) { mMapNodeArray.clear(); }
	void Init(void);
	void Release(void);
	void Update(void);
	void Render(void);
	void Insert(MapNodeLine* pLine)
	{
		assert(pLine);
		mMapNodeArray.push_back(pLine);
	}
	MapNode* GetMapNode(int tileX, int tileY) { return mMapNodeArray[tileY]->GetMapNode(tileX); }

private:
	vector<MapNodeLine*> mMapNodeArray;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class MapManager: public Singleton<MapManager>
{
	typedef vector<MapNodeLine> MapNodeList;
public:
	MapManager(void);
	~MapManager(void);
	void Reset(void);
	
	void Update(void);
	void Render(void);

	void InitMap(void);
	void ReleaseMap(void);

	MapCamera& GetCamera(void) { return mCamera; }
	bool IsActive(void) { return mbActive; }
	
	MapID GetCurMapID(void) { return mCurMapId; }
	MapID GetNextMapID(void) { return mNextMapId; }

	int GetMapWidth() { return mMapWidth; }
	int GetMapHeight() { return mMapHeight; }
	int GetTileXFromPosX(int posX);
	int GetTileYFromPosY(int posY);
	int GetMapData(int tileX, int tileY);
	const MapObjInfo& GetMapObjInfo(int index);

private:
	void UpdatePlayer(void);
	void UpdateCamera(void);

public:
	bool MoveActor(int actorId, int targetTileX, int targetTileY, MapActor::MoveDir moveDir);
	bool TestTriggers(MapActor* pActor);

	MapRenderObj* GetRenderObj(int texId);
	MapTrigger* GetTrigger(int tileX, int tileY);
	MapDetector* GetDetector(int tileX, int tileY);
	MapActor* GetActor(int tileX, int tileY);
	MapActor* GetActor(int actorId);
	int GetTargetActorId(void);
	MapActor::MoveDir GetActorTalkingDir(MapActor* pActor);
	MapDetector* GetTargetDetector(void);

	void SetNextMapInfo(MapID nextMapId, int nextTileX, int nextTileY, int nextDir)
	{
		mNextMapId = nextMapId;
		mNextTileX = nextTileX;
		mNextTileY = nextTileY;
		mNextDir = nextDir;
	}
	void SetTankMapInfo(MapID tankMapId, int tankTileX, int tankTileY, int tankDir)
	{
		mTankMapId = tankMapId;
		mTankTileX = tankTileX;
		mTankTileY = tankTileY;
		mTankDir = tankDir;
	}
	void RemoveMapActor(MapActor* pActor);
	void ReInitMapNode(int tileX, int tileY, int texId, bool actorMove, bool tankMove);

	bool IsPlayerCanGotoTank(void);
	void PlayerGotoTank(bool on);
	bool IsPlayerInTank(void) { return mbIsInTank; }

	void GetActPlayerInfo(short &mapId, short &tilex, short &tiley, short &dir)
	{
		mapId = mCurMapId;
		tilex = mpPlayer->GetTileX();
		tiley = mpPlayer->GetTileY();
		dir = mpPlayer->GetCurDir();
	}
	void GetActTankInfo(short &mapId, short &tilex, short &tiley, short &dir)
	{
		if(mpTank)
		{
			mapId = mCurMapId;
			tilex = mpTank->GetTileX();
			tiley = mpTank->GetTileY();
			dir = mpTank->GetCurDir();
		}
		else
		{
			mapId = mTankMapId;
			tilex = mTankTileX;
			tiley = mTankTileY;
			dir = mTankDir;
		}
	}

	int GetMoveStep(void) { return mMoveAccumStep; }
	void AddMoveStep() { mMoveAccumStep++; mbStepNotice = true; }
	void ClearMoveStep() { mMoveAccumStep = 0; }
	bool IsStepNotice(void) { return mbStepNotice; }
	void ClearStepNotice(void) { mbStepNotice = false; }

	void SetFreezed(bool freeze) { mbFreezed = freeze; }

private:
	void InitMapNodes(void);
	void ReleaseMapNodes(void);

	void InitMapObjects(void);
	void ReleaseMapObjects(void);

	void InitMapRenderObjs(const MapObjInfo& mapObjInfo);
	void ReleaseMapRenderObjs(void);
	void InitMapTriggers(const MapObjInfo& mapObjInfo);
	void ReleaseMapTriggers(void);
	void InitMapActors(const MapObjInfo& mapObjInfo);
	void ReleaseMapActors(void);
	void InitMapDetectors(const MapObjInfo& mapObjInfo);
	void ReleaseMapDetectors(void);

private:
	MapNodeArray mpMapNodes;
	int mMapWidth;
	int mMapHeight;
	
	MapID mCurMapId;
	MapID mNextMapId;
	int mNextTileX;
	int mNextTileY;
	int mNextDir;
	MapID mTankMapId;
	int mTankTileX;
	int mTankTileY;
	int mTankDir;
	MapCamera mCamera;
	bool mbActive;

	MapActor* mpPlayer;
	MapActor* mpTank;
	bool mbIsInTank;

	int mMoveAccumStep;
	bool mbStepNotice;
	bool mbFreezed;

	// To Do: data driven vector<MapObject*>
	vector<MapRenderObj*> mMapRenderObjs;
	vector<MapTrigger*> mMapTriggers;
	vector<MapActor*> mMapActors;
	vector<MapDetector*> mMapDetectors;
};
#define gMapMgr MapManager::GetInstance()

#endif