#include "WanterScene.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/Manager/RenderManager.h"

WanterScene::WanterScene(SceneID id)
:	Scene(id)
{
}

WanterScene::~WanterScene(void)
{
}

bool WanterScene::Load(void)
{
	gTexMgr.SetFileList("../Texture/Scenes/WanterScene/WanterBG.png", TEX_WANTER_BG);
	gTexMgr.LoadTextures();

	mSprBG.SetSize(640.0f, 528.0f);
	mSprBG.SetPosition(0.0f, 0.0f);
	mSprBG.SetTexture(gTexMgr.GetTexture(TEX_WANTER_BG));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_NONE);
	mSprBG.SetAlignMode(Sprite::ALIGNMODE_CENTER);
	gRenderMgr.AddObject(&mSprBG);

	return true;
}

void WanterScene::Release(void)
{
	gTexMgr.ReleaseTextures();
	gRenderMgr.ClearAll();
}

void WanterScene::Update(void)
{
	if(Global::KeyDown(KEY_A) || Global::KeyDown(KEY_B))
	{
		gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
	}
}
