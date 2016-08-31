#include "MapRenderObj.h"
#include "../../Engine/Manager/TextureManager.h"

MapRenderObj::MapRenderObj(void)
:	mTexId(0),
	mVisible(true)
{
}

MapRenderObj::~MapRenderObj(void)
{
}

void MapRenderObj::Init(int texId, int scaleSize)
{
	mTexId = texId;
	mSprRenderObj.SetSize((float)(36*scaleSize), (float)(30*scaleSize));
	mSprRenderObj.SetTexture(gTexMgr.GetSystemTex(mTexId));
	mSprRenderObj.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
}

void MapRenderObj::SetPos(float x, float y)
{
	MapObject::SetPos(x, y);
	mSprRenderObj.SetPosition(x, y);
}

void MapRenderObj::Update(void)
{
	if(mVisible)
	{
		mSprRenderObj.Update();
	}
}

void MapRenderObj::Render(void)
{
	if(mVisible)
	{
		mSprRenderObj.Render();
	}
}