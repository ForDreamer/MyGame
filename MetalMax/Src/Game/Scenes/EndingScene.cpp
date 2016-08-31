#include "EndingScene.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Engine/Manager/RenderManager.h"
#include "../Manager/CharacterMgr.h"

static char tempStr[64];
int EndingScene::sEngStep = 0;

EndingScene::EndingScene(SceneID id)
:	Scene(id)
{
}

EndingScene::~EndingScene(void)
{
}

bool EndingScene::Load(void)
{
	Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);

	if(sEngStep == 0)
	{
		mTxtEnd.SetPosition(-120.0f, 60.0f);
		sprintf(tempStr, "歼灭无数怪物的\n超级勇士%s\n开始了新的旅途..", pPlayer->GetName());
	}
	else if(sEngStep == 1)
	{
		mTxtEnd.SetPosition(-125.0f, 50.0f);
		strcpy_s(tempStr, "Thanks For Playing!\n       Game Over");
	}
	else if(sEngStep == 2)
	{
		mTxtEnd.SetPosition(-180.0f, 120.0f);
		strcpy_s(tempStr, "Main Programmer & Designer\n                神乐游侠");
	}
	mTxtEnd.SetText(tempStr);
	mTxtEnd.SetFontType(FT_BIGFONT);
	gRenderMgr.AddObject(&mTxtEnd);

	if(sEngStep == 0)
	{
		mTxtPresent.SetVisible(false);
		mTxtQQ.SetVisible(false);
		mTxtEmail.SetVisible(false);
	}
	else if(sEngStep == 1)
	{
		mTxtPresent.SetVisible(false);
		mTxtQQ.SetVisible(false);
		mTxtEmail.SetVisible(false);
	}
	else if(sEngStep == 2)
	{
		mTxtPresent.SetVisible(true);
		mTxtQQ.SetVisible(true);
		mTxtEmail.SetVisible(true);
		mTxtPresent.SetText("Present By Symphia 2010");
		mTxtQQ.SetText("QQ: 200509225");
		mTxtEmail.SetText("EMail: Nx_AFylrst@163.com");
	}
	mTxtPresent.SetPosition(-108.0f, -20.0f);
	mTxtQQ.SetPosition(-65.0f, -50.0f);
	mTxtEmail.SetPosition(-120.0f, -80.0f);
	gRenderMgr.AddObject(&mTxtPresent);
	gRenderMgr.AddObject(&mTxtQQ);
	gRenderMgr.AddObject(&mTxtEmail);

	return true;
}

void EndingScene::Release(void)
{
	gRenderMgr.ClearAll();
}

void EndingScene::Update(void)
{
	if(sEngStep == 0)
	{
		if(Global::KeyDown(KEY_A))
		{
			gSceneMgr.SetNextScene(SCENE_ENDING);
			sEngStep++;
		}
	}
	else if(sEngStep == 1)
	{
		if(Global::KeyDown(KEY_A))
		{
			gSceneMgr.SetNextScene(SCENE_ENDING);
			sEngStep++;
		}
	}
	else if(sEngStep == 2)
	{
		if(Global::KeyDown(KEY_A))
		{
			gSceneMgr.SetNextScene(SCENE_STARTUP);
			FmodSound::StopMusic();
			sEngStep = 0;
		}
	}
}
