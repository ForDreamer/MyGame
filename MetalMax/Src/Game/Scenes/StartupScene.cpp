#include "StartupScene.h"
#include "NameSettingScene.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/Manager/RenderManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Main/Global.h"
#include "../MapEngine/MapManager.h"
#include "../Manager/GameStatusMgr.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/BattleTankMgr.h"
#include "../Manager/ItemMgr.h"
#include "../Manager/EquipMgr.h"
#include "../Manager/SaveLoadMgr.h"
#include "../UI/UIManager.h"
#include "../Battle/BattleUIMgr.h"

StartupScene::StartupScene(SceneID id)
:	Scene(id),
	mSelectIndex(0),
	mbHasSaveFile(false)
{
}

StartupScene::~StartupScene(void)
{
}

bool StartupScene::Load(void)
{
	gTexMgr.SetFileList("../Texture/Scenes/StartupScene/StartupBG.png", TEX_STARTUP_BG);
	gTexMgr.LoadTextures();

	mSprLogo.SetSize(576.0f, 480.0f);
	mSprLogo.SetPosition(0.0f, 10.0f);
	mSprLogo.SetTexture(gTexMgr.GetTexture(TEX_STARTUP_BG));
	mSprLogo.SetBlendMode(Sprite::BLENDMODE_NONE);
	mSprLogo.SetAlignMode(Sprite::ALIGNMODE_CENTER);
	gRenderMgr.AddObject(&mSprLogo);

	mTxtInfo.SetPosition(-125.0f, -190.0f);
	mTxtInfo.SetText("Presented by Symphia 2010");
	gRenderMgr.AddObject(&mTxtInfo);
	

	mTxtNewGame.SetPosition(-40.0f, -40.0f);
	mTxtNewGame.SetText("����Ϸ");
	mTxtNewGame.SetFontType(FT_BIGFONT);
	gRenderMgr.AddObject(&mTxtNewGame);

	mTxtContinue.SetPosition(-40.0f, -85.0f);
	mTxtContinue.SetText("������Ϸ");
	mTxtContinue.SetFontType(FT_BIGFONT);
	gRenderMgr.AddObject(&mTxtContinue);

	mTxtExit.SetPosition(-40.0f, -130.0f);
	mTxtExit.SetText("�˳�");
	mTxtExit.SetFontType(FT_BIGFONT);
	gRenderMgr.AddObject(&mTxtExit);

	mIcon.SetSize(36.0f, 30.0f);
	mIcon.SetPosition(-60.0f, -56.0f);
	mIcon.SetTexture(gTexMgr.GetSystemTex(TEX_SYS_SELECTOR_ICON));
	mIcon.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	gRenderMgr.AddObject(&mIcon);

	
	FILE* fp = fopen(sSaveFilename, "rt") ;
	if(fp)
	{
		mTxtContinue.SetColor(0xffffffff);
		mbHasSaveFile = true;
		fclose(fp);
	}
	else
	{
		mTxtContinue.SetColor(0xff7f7f7f);
		mbHasSaveFile = false;
	}

	FmodSound::PlayMusic(BGM_STARTUP);
	return true;
}

void StartupScene::Release(void)
{
	FmodSound::StopMusic();

	gTexMgr.ReleaseTextures();
	gRenderMgr.ClearAll();
}
/************************************************************************/
/* ��ʼ������ת                                                           */
/************************************************************************/
void StartupScene::Update(void)
{
	//�ϼ�
	if(Global::KeyDown(KEY_UP))
	{
		if(mSelectIndex > 0)
		{
			mSelectIndex--;
			ResetIconPos();
			FmodSound::PlaySnd(SND_SELECT);
		}
	}
	//�¼�
	else if(Global::KeyDown(KEY_DOWN))
	{
		if(mSelectIndex < 2)
		{
			mSelectIndex++;
			ResetIconPos();
			FmodSound::PlaySnd(SND_SELECT);
		}
	}
	//ȷ����ť
	if(Global::KeyDown(KEY_A))
	{
		//��ʼ
		if(mSelectIndex == 0)
		{
			gSceneMgr.SetNextScene(SCENE_NAMESETTING);
			NameSettingScene::sUserType = UT_PLAYER;
			FmodSound::PlaySnd(SND_CONFIRM);

			StartGame();
			gSaveLoadMgr.ResetSaveData();
			gSaveLoadMgr.LoadSaveData_ToGame();
		}
		//����
		else if(mSelectIndex == 1)
		{
			if(mbHasSaveFile)
			{
				gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
				FmodSound::PlaySnd(SND_CONFIRM);
				
				StartGame();
				gSaveLoadMgr.Load();
			}
		}
		//�˳�
		else if(mSelectIndex == 2)
		{
			DestroyWindow(Global::GetHWnd());
		}
	}
}

void StartupScene::ResetIconPos(void)
{
	int posY = -56 - 45*mSelectIndex;
	mIcon.SetPositionY((float)posY);
}
//��ʼ��Ϸ
void StartupScene::StartGame(void)
{
	gUIMgr.CloseAllUI();
	gBattleUIMgr.CloseAllUI();
	gMapMgr.Reset();
	gItemMgr.Reset();
	gEquipMgr.Reset();
	gGameStatusMgr.Reset();
}