#include "NameSettingScene.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Engine/Manager/RenderManager.h"
#include "../../Engine/FmodSound/FmodSound.h"
#include "../../Main/Global.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/BattleTankMgr.h"

static const NameSettingScene::KeyInfo sKeyInfos[NS_KEY_NUM] = 
{
	{"0"},{" "},{"1"},{" "},{"2"},  {"3"},{" "},{"4"},{" "},{"5"},  {"6"},{" "},{"7"},{" "},{"8"},  {"9"},{" "},{"A"},{" "},{"B"},
	{"C"},{" "},{"D"},{" "},{"E"},  {"F"},{" "},{"G"},{" "},{"H"},  {"I"},{" "},{"J"},{" "},{"K"},  {"L"},{" "},{"M"},{" "},{"N"},
	{"O"},{" "},{"P"},{" "},{"Q"},  {"R"},{" "},{"S"},{" "},{"T"},  {"U"},{" "},{"V"},{" "},{"W"},  {"X"},{" "},{"Y"},{" "},{"Z"},
	{"a"},{" "},{"b"},{" "},{"c"},  {"d"},{" "},{"e"},{" "},{"f"},  {"g"},{" "},{"h"},{" "},{"i"},  {"j"},{" "},{"k"},{" "},{"l"},
	{"m"},{" "},{"n"},{" "},{"o"},  {"p"},{" "},{"q"},{" "},{"r"},  {"s"},{" "},{"t"},{" "},{"u"},  {"v"},{" "},{"w"},{" "},{"x"},
	{"y"},{" "},{"z"},{" "}
};
static const int sPosXOffset[NS_KEY_NUM] = 
{
	0,0,0,0,0,		0,0,0,0,0,		0,0,0,0,0,		0,0,0,0,0,
	-1,0,0,0,0,		0,0,-2,0,-1,	3,0,0,0,0,		0,0,-2,0,0,
	-2,0,0,0,-2,	0,0,0,0,0,		0,0,0,0,-2,		0,0,0,0,1,
	0,0,0,0,0,		0,0,0,0,0,		0,0,0,0,0,		2,0,0,0,2,
	-2,0,0,0,0,		0,0,0,0,0,		1,0,3,0,-2,		0,0,0,0,0,
	0,0,0,0
};

UserType NameSettingScene::sUserType = UT_PLAYER;

NameSettingScene::NameSettingScene(SceneID id)
:	Scene(id),
	mCurKeyIndex(0),
	mCurEditIndex(0),
	mfCurXAccum(0.0f),
	mfCurYAccum(0.0f)
{
	for(int i=0; i<NS_KEY_EIDT_MAX; i++)
	{
		mName[i] = ' ';
	}
}

NameSettingScene::~NameSettingScene(void)
{
}

void NameSettingScene::InitKeyBoard(void)
{
	const int COL_MAX = 20;
	const int ROW_MAX = 6;
	const int COL_X[COL_MAX] = {-240,-225,-210,-195,-180, -110,-95,-80,-65,-50, 20,35,50,65,80, 150,165,180,195,210};
	const int ROW_Y[ROW_MAX] = {90,50,10,-30,-70,-110};
	int colIndex = 0;
	int rowIndex = 0;

	for(int i=0; i<NS_KEY_NUM; i++)
	{
		colIndex = i%COL_MAX;
		rowIndex = i/COL_MAX;
		mKeys[i].SetPosition((float)(COL_X[colIndex] + sPosXOffset[i]), (float)ROW_Y[rowIndex]);
		mKeys[i].SetText(sKeyInfos[i].key);
		gRenderMgr.AddObject(&mKeys[i]);
	}
}

void NameSettingScene::InitTopText(void)
{
	for(int i=0; i<NS_KEY_EIDT_MAX; i++)
	{
		gRenderMgr.AddObject(&mTxtName[i]);
	}
}

void NameSettingScene::UpdateTopText(void)
{
	char strTemp[2] = {0};
	float posX[NS_KEY_EIDT_MAX] = {-35.0f, -15.0f, 5.0f, 25.0f};
	for(int i=0; i<NS_KEY_EIDT_MAX; i++)
	{
		sprintf_s(strTemp, "%c", mName[i]);
		mTxtName[i].SetText(strTemp, true);

		float offsetX = 0.0f;
		for(int k=0; k<NS_KEY_NUM; k++)
		{
			if(sKeyInfos[k].key[0] == mName[i])
				offsetX = (float)(sPosXOffset[k]);
		}
		mTxtName[i].SetPosition(posX[i] + offsetX - 1.0f, 153.0f);
	}
}

bool NameSettingScene::Load(void)
{
	gTexMgr.SetFileList("../Texture/Scenes/NameSettingScene/NameSettingBG.png", TEX_NAMESETTING_BG);
	gTexMgr.SetFileList("../Texture/Scenes/NameSettingScene/IconTop.png", TEX_ICON_TOP);
	gTexMgr.SetFileList("../Texture/Scenes/NameSettingScene/IconBot.png", TEX_ICON_BOT);
	gTexMgr.LoadTextures();

	mSprBG.SetSize(640.0f, 480.0f);
	mSprBG.SetPosition(0.0f, 0.0f);
	mSprBG.SetTexture(gTexMgr.GetTexture(TEX_NAMESETTING_BG));
	mSprBG.SetBlendMode(Sprite::BLENDMODE_NONE);
	gRenderMgr.AddObject(&mSprBG);

	if(sUserType == UT_PLAYER)
	{
		mTxtInfo.SetPosition(-50.0f, 195.0f);
		mTxtInfo.SetText("起 名 ？");
	}
	else if(sUserType == UT_TANK)
	{
		mTxtInfo.SetPosition(-80.0f, 195.0f);
		mTxtInfo.SetText("战 车 起 名 ？");
	}
	mTxtInfo.SetFontType(FT_BIGFONT);
	gRenderMgr.AddObject(&mTxtInfo);

	mTxtBack.SetPosition(183.0f, -138.0f);
	mTxtBack.SetText("BACK");
	gRenderMgr.AddObject(&mTxtBack);

	mTxtEnd.SetPosition(183.0f, -164.0f);
	mTxtEnd.SetText("END");
	gRenderMgr.AddObject(&mTxtEnd);

	InitKeyBoard();
	InitTopText();
	UpdateTopText();

	mIconTop.SetSize(72.0f, 60.0f);
	mIconTop.SetTexture(gTexMgr.GetTexture(TEX_ICON_TOP));
	mIconTop.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	gRenderMgr.AddObject(&mIconTop);

	mIconBot.SetSize(72.0f, 60.0f);
	mIconBot.SetTexture(gTexMgr.GetTexture(TEX_ICON_BOT));
	mIconBot.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	gRenderMgr.AddObject(&mIconBot);

	ResetSprTopPos();
	ResetSprBotPos();


	FmodSound::PlayMusic(BGM_NAMESETTING);

	return true;
}

void NameSettingScene::Release(void)
{
	FmodSound::StopMusic();

	gTexMgr.ReleaseTextures();
	gRenderMgr.ClearAll();
}

void NameSettingScene::GotoNextScene(void)
{
	gSceneMgr.SetNextScene(SCENE_MAPNAVIGATION);
}

void NameSettingScene::Update(void)
{
	if(Global::KeyPressed(KEY_UP))
	{
		if(mCurKeyIndex < NS_KEY_NUM)
		{
			mfCurYAccum += 7.5f * Global::GetTimeDelta();
			if(mfCurYAccum > 1.0f)
			{
				mfCurYAccum = 0.0f;

				if(mCurKeyIndex >= 20)
					mCurKeyIndex -= 20;
				ResetSprBotPos();
			}
		}
	}
	else if(Global::KeyPressed(KEY_DOWN))
	{
		if(mCurKeyIndex < NS_KEY_NUM)
		{
			mfCurYAccum += 7.5f * Global::GetTimeDelta();
			if(mfCurYAccum > 1.0f)
			{
				mfCurYAccum = 0.0f;

				mCurKeyIndex += 20;
				if(mCurKeyIndex >= NS_KEY_NUM)
				{
					mCurKeyIndex = NS_KEY_BACK;
				}
				ResetSprBotPos();
			}
		}
	}

	if(Global::KeyDown(KEY_UP))
	{
		if(mCurKeyIndex == NS_KEY_BACK)
			mCurKeyIndex = 99;
		else if(mCurKeyIndex == NS_KEY_END)
			mCurKeyIndex = NS_KEY_BACK;
		ResetSprBotPos();
	}
	else if(Global::KeyDown(KEY_DOWN))
	{
		if(mCurKeyIndex == NS_KEY_BACK)
			mCurKeyIndex = NS_KEY_END;
		ResetSprBotPos();
	}

	if(Global::KeyPressed(KEY_LEFT))
	{
		if(mCurKeyIndex < NS_KEY_NUM)
		{
			mfCurXAccum += 10.0f * Global::GetTimeDelta();
			if(mfCurXAccum > 1.0f)
			{
				mfCurXAccum = 0.0f;
			
				mCurKeyIndex--;
				if(mCurKeyIndex < 0)
					mCurKeyIndex = 0;
				ResetSprBotPos();
			}
		}
	}
	else if(Global::KeyPressed(KEY_RIGHT))
	{
		if(mCurKeyIndex < NS_KEY_NUM)
		{
			mfCurXAccum += 10.0f * Global::GetTimeDelta();
			if(mfCurXAccum > 1.0f)
			{
				mfCurXAccum = 0.0f;
				
				mCurKeyIndex++;
				if(mCurKeyIndex >= NS_KEY_NUM)
					mCurKeyIndex = NS_KEY_BACK;
				ResetSprBotPos();
			}
		}
	}

	if(Global::KeyDown(KEY_A))
	{
		if(mCurKeyIndex < NS_KEY_NUM)
		{
			mName[mCurEditIndex] = sKeyInfos[mCurKeyIndex].key[0];
			if(mCurEditIndex < NS_KEY_EIDT_MAX-1)
			{
				mCurEditIndex++;
				ResetSprTopPos();
			}
			UpdateTopText();
		}
		else if(mCurKeyIndex == NS_KEY_BACK)
		{
			mName[mCurEditIndex] = sKeyInfos[NS_KEY_NUM-1].key[0];
			if(mCurEditIndex > 0)
			{
				mCurEditIndex--;
				ResetSprTopPos();
			}
			UpdateTopText();
		}
		else if(mCurKeyIndex == NS_KEY_END)
		{
			GotoNextScene();
			if(sUserType == UT_PLAYER)
			{
				if(mName[0] == ' ' && mName[1] == ' ' && mName[2] == ' ' && mName[3] == ' ')
				{
					mName[0] = 'N';
					mName[1] = 'x';
					mName[2] = 'A';
					mName[3] = 'F';
				}
				Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
				pPlayer->SetName(mName);
			}
			else if(sUserType == UT_TANK)
			{
				if(mName[0] == ' ' && mName[1] == ' ' && mName[2] == ' ' && mName[3] == ' ')
				{
					mName[0] = 'T';
					mName[1] = 'a';
					mName[2] = 'n';
					mName[3] = 'k';
				}
				BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
				pTank->SetName(mName);
			}
		}
		FmodSound::PlaySnd(SND_CONFIRM);
	}
	else if(Global::KeyDown(KEY_B))
	{
		mName[mCurEditIndex] = sKeyInfos[NS_KEY_NUM-1].key[0];
		if(mCurEditIndex > 0)
		{
			mCurEditIndex--;
			ResetSprTopPos();
		}
		UpdateTopText();
		FmodSound::PlaySnd(SND_CONFIRM);
	}
}

void NameSettingScene::ResetSprBotPos(void)
{
	if(mCurKeyIndex < NS_KEY_NUM)
	{
		float x = mKeys[mCurKeyIndex].GetPositionX() + 7.0f - sPosXOffset[mCurKeyIndex];
		float y = mKeys[mCurKeyIndex].GetPositionY() - 9.0f;
		mIconBot.SetPosition(x, y);
	}
	else if(mCurKeyIndex == NS_KEY_BACK)
	{
		mIconBot.SetPosition(170.0f, -147.0f);
	}
	else if(mCurKeyIndex == NS_KEY_END)
	{
		mIconBot.SetPosition(170.0f, -173.0f);
	}
}

void NameSettingScene::ResetSprTopPos(void)
{
	float x = (float)(-29 + mCurEditIndex * 20);
	mIconTop.SetPosition(x, 133.0f);
}