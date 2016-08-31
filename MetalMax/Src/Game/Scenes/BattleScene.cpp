#include "BattleScene.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Main/Global.h"
#include "../Battle/BattleUIMgr.h"
#include "../Battle/BattleManager.h"

BattleScene::BattleScene(SceneID id)
:	Scene(id)
{
}

BattleScene::~BattleScene(void)
{
}

bool BattleScene::Load(void)
{
	gBattleMgr.Init();

	FmodSound::StopSnd(SND_STARTBATTLE);
	if(gBattleMgr.IsBossFight())
		FmodSound::PlayMusic(BGM_BOSSFIGHT);
	else
		FmodSound::PlayMusic(BGM_BATTLETHEME);

	return true;
}

void BattleScene::Release(void)
{
	gBattleUIMgr.CloseAllUI();

	gBattleMgr.Release();
}

void BattleScene::Update(void)
{
}
