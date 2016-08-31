#include "MapDetector.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../Manager/GameStatusMgr.h"

MapDetector::MapDetector(void)
:	mType(DT_WANTER)
{
}

MapDetector::~MapDetector(void)
{
}

void MapDetector::Init(int type, int unlockId)
{
	mType = DetectorType(type);
	mUnlockId = unlockId;

	if(type <= 2 || type == 6)
	{
		mSprItem.SetVisible(false);
	}
	else
	{
		mSprItem.SetVisible(true);
		mSprItem.SetSize(36.0f, 30.0f);
		mSprItem.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
		if(gGameStatusMgr.IsItemUnlocked(mUnlockId))
			mSprItem.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_ITEMBOX2));
		else
			mSprItem.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_ITEMBOX1));
	}
}

void MapDetector::SetItemBoxUnlock(void)
{
	if(mType <= 2)
		return;
	gGameStatusMgr.SetItemUnlocked(mUnlockId);
	mSprItem.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_ITEMBOX2));
}

bool MapDetector::IsItemBoxUnlocked(void)
{
	if(mType <= 2)
		return false;
	return gGameStatusMgr.IsItemUnlocked(mUnlockId);
}


void MapDetector::SetPos(float x, float y)
{
	MapObject::SetPos(x, y);
	mSprItem.SetPosition(x, y);
}

void MapDetector::Update(void)
{
	mSprItem.Update();
}

void MapDetector::Render(void)
{
	mSprItem.Render();
}