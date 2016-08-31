#include "InnScene.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Main/Global.h"
#include "../Manager/CharacterMgr.h"

InnScene::InnScene(SceneID id)
:	Scene(id),
	mTimer(0.0f)
{
}

InnScene::~InnScene(void)
{
}

bool InnScene::Load(void)
{
	mTimer = 0.0f;
	FmodSound::PlaySnd(SND_INN);
	Character* pCharacter = gCharacterMgr.GetCharacter(CHARID_PLAYER);
	pCharacter->SetAttrib(CAB_HP, pCharacter->GetAttrib(CAB_HPMAX));
	return true;
}

void InnScene::Release(void)
{
}

void InnScene::Update(void)
{
	mTimer += Global::GetTimeDelta();
	if(mTimer > 2.2f)
	{
		gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
	}
}
