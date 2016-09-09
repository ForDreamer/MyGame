#include "TextureManager.h"
#include "../../Main/Global.h"
#include <assert.h>
#include <strsafe.h>

DECLARE_SINGLETON(TextureManager);
static TextureManager sTextureManager;

static const char* sSysTexNames[TEX_SYS_MAX] =
{
	"../Texture/System/IconSelector.png",
	"../Texture/System/IconTest.png",
	"../Texture/System/BarDoor.png",
	"../Texture/System/TecDoor.png",
	"../Texture/System/InnDoor.png",
	"../Texture/System/TextCursor.png",
	"../Texture/System/ItemBox1.png",
	"../Texture/System/ItemBox2.png"
};

// To Do: optimize
static const char* sActTexNames[TEX_ACT_MAX] =
{
	"../Texture/Actors/Player/Player_Walk_L.png",
	"../Texture/Actors/Player/Player_Walk_U.png",
	"../Texture/Actors/Player/Player_Walk_R.png",
	"../Texture/Actors/Player/Player_Walk_D.png",

	"../Texture/Actors/NPC1/NPC1_Walk_L.png",
	"../Texture/Actors/NPC1/NPC1_Walk_U.png",
	"../Texture/Actors/NPC1/NPC1_Walk_R.png",
	"../Texture/Actors/NPC1/NPC1_Walk_D.png",

	"../Texture/Actors/NPC2/NPC2_Walk_L.png",
	"../Texture/Actors/NPC2/NPC2_Walk_U.png",
	"../Texture/Actors/NPC2/NPC2_Walk_R.png",
	"../Texture/Actors/NPC2/NPC2_Walk_D.png",

	"../Texture/Actors/NPC3/NPC3_Walk_L.png",
	"../Texture/Actors/NPC3/NPC3_Walk_U.png",
	"../Texture/Actors/NPC3/NPC3_Walk_R.png",
	"../Texture/Actors/NPC3/NPC3_Walk_D.png",

	"../Texture/Actors/NPC4/NPC4_Walk_L.png",
	"../Texture/Actors/NPC4/NPC4_Walk_U.png",
	"../Texture/Actors/NPC4/NPC4_Walk_R.png",
	"../Texture/Actors/NPC4/NPC4_Walk_D.png",

	"../Texture/Actors/NPC5/NPC5_Walk_L.png",
	"../Texture/Actors/NPC5/NPC5_Walk_U.png",
	"../Texture/Actors/NPC5/NPC5_Walk_R.png",
	"../Texture/Actors/NPC5/NPC5_Walk_D.png",

	"../Texture/Actors/NPC6/NPC6_Walk_L.png",
	"../Texture/Actors/NPC6/NPC6_Walk_U.png",
	"../Texture/Actors/NPC6/NPC6_Walk_R.png",
	"../Texture/Actors/NPC6/NPC6_Walk_D.png",

	"../Texture/Actors/NPC7/NPC7_Walk_L.png",
	"../Texture/Actors/NPC7/NPC7_Walk_U.png",
	"../Texture/Actors/NPC7/NPC7_Walk_R.png",
	"../Texture/Actors/NPC7/NPC7_Walk_D.png",

	"../Texture/Actors/NPC8/NPC8_Walk_L.png",
	"../Texture/Actors/NPC8/NPC8_Walk_U.png",
	"../Texture/Actors/NPC8/NPC8_Walk_R.png",
	"../Texture/Actors/NPC8/NPC8_Walk_D.png",

	"../Texture/Actors/NPC9/NPC9_Walk_L.png",
	"../Texture/Actors/NPC9/NPC9_Walk_U.png",
	"../Texture/Actors/NPC9/NPC9_Walk_R.png",
	"../Texture/Actors/NPC9/NPC9_Walk_D.png",

	"../Texture/Actors/NPC10/NPC10_Walk_L.png",
	"../Texture/Actors/NPC10/NPC10_Walk_U.png",
	"../Texture/Actors/NPC10/NPC10_Walk_R.png",
	"../Texture/Actors/NPC10/NPC10_Walk_D.png",

	"../Texture/Actors/NPC11/NPC11_Walk_L.png",
	"../Texture/Actors/NPC11/NPC11_Walk_U.png",
	"../Texture/Actors/NPC11/NPC11_Walk_R.png",
	"../Texture/Actors/NPC11/NPC11_Walk_D.png",

	"../Texture/Actors/NPC12/NPC12_Walk_L.png",
	"../Texture/Actors/NPC12/NPC12_Walk_U.png",
	"../Texture/Actors/NPC12/NPC12_Walk_R.png",
	"../Texture/Actors/NPC12/NPC12_Walk_D.png",

	"../Texture/Actors/NPC13/NPC13_Walk_L.png",
	"../Texture/Actors/NPC13/NPC13_Walk_U.png",
	"../Texture/Actors/NPC13/NPC13_Walk_R.png",
	"../Texture/Actors/NPC13/NPC13_Walk_D.png",

	"../Texture/Actors/NPC14/NPC14_Walk_L.png",
	"../Texture/Actors/NPC14/NPC14_Walk_U.png",
	"../Texture/Actors/NPC14/NPC14_Walk_R.png",
	"../Texture/Actors/NPC14/NPC14_Walk_D.png",

	"../Texture/Actors/NPC15/NPC15_Walk_L.png",
	"../Texture/Actors/NPC15/NPC15_Walk_U.png",
	"../Texture/Actors/NPC15/NPC15_Walk_R.png",
	"../Texture/Actors/NPC15/NPC15_Walk_D.png",

	"../Texture/Actors/Tank/Tank_Walk_L.png",
	"../Texture/Actors/Tank/Tank_Walk_U.png",
	"../Texture/Actors/Tank/Tank_Walk_R.png",
	"../Texture/Actors/Tank/Tank_Walk_D.png"
};

static const char* sUITexNames[TEX_UI_MAX] =
{
	"../Texture/UI/UIM_Item0.png",
	"../Texture/UI/UIM_Item1.png",
	"../Texture/UI/UIM_Item2.png",
	"../Texture/UI/UIM_Item3.png",
	"../Texture/UI/UIM_Item4.png",
	"../Texture/UI/UIM_Item5.png",
	"../Texture/UI/UIM_Item6.png",
	"../Texture/UI/UIM_Item7.png",

	"../Texture/UI/UIE_Gun.png",
	"../Texture/UI/UIE_Cloth.png",
	"../Texture/UI/UIE_Shoe.png",
	"../Texture/UI/UIE_Hand.png",

	"../Texture/UI/UI_Main.png",
	"../Texture/UI/UI_Textbox.png",
	"../Texture/UI/UI_Cannon.png",
	"../Texture/UI/UI_Status.png",
	"../Texture/UI/UI_Top.png",

	"../Texture/UI/BatUI_HumMain.png",
	"../Texture/UI/BatUI_ItemHum_0.png",
	"../Texture/UI/BatUI_ItemHum_1.png",
	"../Texture/UI/BatUI_ItemHum_2.png",

	"../Texture/UI/BatUI_TankMain.png",
	"../Texture/UI/BatUI_ItemTank_0.png",
	"../Texture/UI/BatUI_ItemTank_1.png",
	"../Texture/UI/BatUI_ItemTank_2.png",
	"../Texture/UI/BatUI_ItemTank_3.png",
	"../Texture/UI/BatUI_InfoBox.png"
};

static const char* sBatTexNames[TEX_BAT_MAX] =
{
	"../Texture/Battle/Bat_Ant.png",
	"../Texture/Battle/Bat_Bug.png",
	"../Texture/Battle/Bat_Snail.png",
	"../Texture/Battle/Bat_Fire.png",
	"../Texture/Battle/Bat_Gun.png",
	"../Texture/Battle/Bat_Cannon.png",
	"../Texture/Battle/Bat_Boss.png",
	"../Texture/Battle/Bat_Player.png",
	"../Texture/Battle/Bat_Tank.png",
	"../Texture/Battle/Bat_Die.png",
	"../Texture/Battle/Bat_Player_Gun.png",
	"../Texture/Battle/Bat_Effect_Gun_1.png",
	"../Texture/Battle/Bat_Effect_Gun_2.png",
	"../Texture/Battle/Bat_Player_Sling.png",
	"../Texture/Battle/Bat_Player_Tool.png",
	"../Texture/Battle/Bat_Sling_Shot.png",
	"../Texture/Battle/Bat_Bomb_Shot.png",
	"../Texture/Battle/Bat_Fire_Shot.png",
	"../Texture/Battle/Bat_Fire_Shot_2.png",
	"../Texture/Battle/Bat_Cannon_Shot.png",
	"../Texture/Battle/Bat_Cannon_Shot_2.png",
	"../Texture/Battle/Bat_Effect_Bomb.png",
	"../Texture/Battle/Bat_Effect_Fire.png",
	"../Texture/Battle/Bat_Effect_Cannon.png"
};



TextureManager::TextureManager(void)
{
	for(int i=0; i<MAX_TEXTURES; i++)
	{
		mpFileList[i] = NULL;
		mpTextures[i] = NULL;
	}

	for(int i=0; i<TEX_SYS_MAX; i++)
		mpSystemTex[i] = NULL;

	for(int i=0; i<MT_MAX; i++)
		mpTileTex[i] = NULL;

	for(int i=0; i<TEX_ACT_MAX; i++)
		mpActorTex[i] = NULL;

	for(int i=0; i<TEX_UI_MAX; i++)
		mpUITex[i] = NULL;

	for(int i=0; i<TEX_BAT_MAX; i++)
		mpBattleTex[i] = NULL;
}
	
TextureManager::~TextureManager(void)
{
}

void TextureManager::SetFileList(const char* filename, int index)
{
	assert(index < MAX_TEXTURES);
	assert(mpFileList[index] == NULL);

	mpFileList[index] = (char*)filename;
}

bool TextureManager::LoadTextures(void)
{
	bool ret = true;
	for(int i=0; i<MAX_TEXTURES; i++)
	{
		if(mpFileList[i])
		{
			if(FAILED(D3DXCreateTextureFromFile(Global::GetDevice(), mpFileList[i], &mpTextures[i])))
			{
				char errStr[128];
				sprintf_s(errStr, "Failed to load texture file: %s", mpFileList[i]);
				MessageBox(0, errStr, 0, 0);
				ret = false;
			}
		}
	}
	return ret;
}
/************************************************************************/
/* 加载系统的Textures资源                                                      */
/************************************************************************/
bool TextureManager::LoadSystemTextures(void)
{
	bool ret = true;
	for(int i=0; i<TEX_SYS_MAX; i++)
	{
		if (FAILED(D3DXCreateTextureFromFile(Global::GetDevice(), sSysTexNames[i], &mpSystemTex[i])))
		{
			char errStr[128];
			sprintf_s(errStr, "Failed to load texture file: %s", sSysTexNames[i]);
			MessageBox(0, errStr, 0, 0);
			ret = false;
		}
	}
	return ret;
}
/************************************************************************/
/* 加载世界Textures资源                                                   */
/************************************************************************/
bool TextureManager::LoadTileTextures(void)
{
	bool ret = true;
	for(int i=0; i<MT_MAX; i++)
	{
		if (FAILED(D3DXCreateTextureFromFile(Global::GetDevice(), sMapTexInfos[i].texName, &mpTileTex[i])))
		{
			char errStr[128];
			sprintf_s(errStr, "Failed to load texture file: %s", sMapTexInfos[i].texName);
			MessageBox(0, errStr, 0, 0);
			ret = false;
		}
	}
	return ret;
}
/************************************************************************/
/* 加载人物Textures资源													*/
/************************************************************************/
bool TextureManager::LoadActorTextures(void)
{
	bool ret = true;
	for(int i=0; i<TEX_ACT_MAX; i++)
	{
		if (FAILED(D3DXCreateTextureFromFile(Global::GetDevice(), sActTexNames[i], &mpActorTex[i])))
		{
			char errStr[128];
			sprintf_s(errStr, "Failed to load texture file: %s", sActTexNames[i]);
			MessageBox(0, errStr, 0, 0);
			ret = false;
		}
	}
	return ret;
}
/************************************************************************/
/* 加载操作UI Textures资源												*/
/************************************************************************/
bool TextureManager::LoadUITextures(void)
{
	bool ret = true;
	for(int i=0; i<TEX_UI_MAX; i++)
	{
		if (FAILED(D3DXCreateTextureFromFile(Global::GetDevice(), sUITexNames[i], &mpUITex[i])))
		{
			char errStr[128];
			sprintf_s(errStr, "Failed to load texture file: %s", sUITexNames[i]);
			MessageBox(0, errStr, 0, 0);
			ret = false;
		}
	}
	return ret;
}
/************************************************************************/
/* 加载战斗画面Textures 资源												*/
/************************************************************************/
bool TextureManager::LoadBattleTextures(void)
{
	bool ret = true;
	for(int i=0; i<TEX_BAT_MAX; i++)
	{
		if (FAILED(D3DXCreateTextureFromFile(Global::GetDevice(), sBatTexNames[i], &mpBattleTex[i])))
		{
			char errStr[128];
			sprintf_s(errStr, "Failed to load texture file: %s", sBatTexNames[i]);
			MessageBox(0, errStr, 0, 0);
			ret = false;
		}
	}
	return ret;
}



void TextureManager::ReleaseTextures(void)
{
	for(int i=0; i<MAX_TEXTURES; i++)
	{
		mpFileList[i] = NULL;
		if(mpTextures[i])
		{
			mpTextures[i]->Release();
			mpTextures[i] = NULL;
		}
	}
}

void TextureManager::ReleaseSystemTexures(void)
{
	for(int i=0; i<TEX_SYS_MAX; i++)
	{
		if(mpSystemTex[i])
		{
			mpSystemTex[i]->Release();
			mpSystemTex[i] = NULL;
		}
	}
}

void TextureManager::ReleaseTileTexures(void)
{
	for(int i=0; i<MT_MAX; i++)
	{
		if(mpTileTex[i])
		{
			mpTileTex[i]->Release();
			mpTileTex[i] = NULL;
		}
	}
}

void TextureManager::ReleaseActorTexures(void)
{
	for(int i=0; i<TEX_ACT_MAX; i++)
	{
		if(mpActorTex[i])
		{
			mpActorTex[i]->Release();
			mpActorTex[i] = NULL;
		}
	}
}

void TextureManager::ReleaseUITexures(void)
{
	for(int i=0; i<TEX_UI_MAX; i++)
	{
		if(mpUITex[i])
		{
			mpUITex[i]->Release();
			mpUITex[i] = NULL;
		}
	}
}

void TextureManager::ReleaseBattleTexures(void)
{
	for(int i=0; i<TEX_BAT_MAX; i++)
	{
		if(mpBattleTex[i])
		{
			mpBattleTex[i]->Release();
			mpBattleTex[i] = NULL;
		}
	}
}