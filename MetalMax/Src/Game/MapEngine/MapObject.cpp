#include "MapObject.h"
#include "MapManager.h"

MapObject::MapObject(void)
:	mPosX(0),
	mPosY(0),
	mTileX(0),
	mTileY(0)
{
}

MapObject::~MapObject(void)
{
}

void MapObject::SetPos(float x, float y)
{
	mPosX = x;
	mPosY = y;
	mTileX = gMapMgr.GetTileXFromPosX((int)mPosX);
	mTileY = gMapMgr.GetTileYFromPosY((int)mPosY);
}