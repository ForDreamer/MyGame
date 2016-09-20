#include "MapNode.h"
#include "MapConfig.h"
#include "../../Engine/Manager/TextureManager.h"

MapNode::MapNode(void)
:	mTexId(MT_BARRIER_1),
	mbActorMovable(false),
	mbTankMovable(false)
{
}

MapNode::~MapNode(void)
{
}

void MapNode::Init(int texID, bool actorMovable, bool tankMovable)
{
	mTexId = (MapTexID)texID;
	mSprTile.SetSize(36.0f+1.0f, 30.0f+1.0f);
	mSprTile.SetTexture(gTexMgr.GetTileTex(mTexId));
	//如果是水域，则展现动画效果
	if(texID >= MT_MAP_11 && texID <= MT_MAP_21)
	{
		mSprTile.SetSize(36.0f+2.0f, 30.0f+2.0f);
		mSprTile.SetAnimator(2, 2, 4, 1, 1.0f);
		mSprTile.PlayAnim();
	}

	mbActorMovable = actorMovable;
	mbTankMovable = tankMovable;
}

void MapNode::ReInit(int texID, bool actorMovable, bool tankMovable)
{
	mTexId = (MapTexID)texID;
	mSprTile.SetTexture(gTexMgr.GetTileTex(mTexId));

	mbActorMovable = actorMovable;
	mbTankMovable = tankMovable;
}

void MapNode::SetPos(float x, float y)
{
	MapObject::SetPos(x, y);
	mSprTile.SetPosition(x, y);
}

void MapNode::Update(void)
{
	mSprTile.Update();
}

void MapNode::Render(void)
{
	mSprTile.Render();
}