#include "MapCamera.h"
#include "../../Main/Global.h"
#include "../../Engine/Scene/Scene.h"

MapCamera::MapCamera(void)
{
}

MapCamera::~MapCamera(void)
{
}

void MapCamera::SetPos(float x, float y)
{
	MapObject::SetPos(x, y);

	D3DXVECTOR3 vEyePt(mPosX, mPosY, -20.0f);
	D3DXVECTOR3 vLookatPt(mPosX, mPosY, 20.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	Global::GetDevice()->SetTransform(D3DTS_VIEW, &matView);

	gSceneMgr.GetFader()->SetPosition(mPosX, mPosY);
}