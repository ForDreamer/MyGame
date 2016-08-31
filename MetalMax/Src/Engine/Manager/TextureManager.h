#ifndef _TextureManager_H_
#define _TextureManager_H_

#include "../Misc/Singleton.h"
#include <d3dx9.h>
#include "../../Game/MapEngine/MapTexDef.h"

#define MAX_TEXTURES 32

enum SystemTextureIndex
{
	TEX_SYS_SELECTOR_ICON = 0,
	TEX_SYS_TEST_ICON = 1,
	TEX_SYS_BAR_DOOR = 2,
	TEX_SYS_TEC_DOOR = 3,
	TEX_SYS_INN_DOOR = 4,
	TEX_SYS_TEXT_CURSOR = 5,
	TEX_SYS_ITEMBOX1 = 6,
	TEX_SYS_ITEMBOX2 = 7,

	TEX_SYS_MAX
};

enum ActorTextureIndex
{
	TEX_ACT_PLAYER_WALK_L = 0,
	TEX_ACT_PLAYER_WALK_U = 1,
	TEX_ACT_PLAYER_WALK_R = 2,
	TEX_ACT_PLAYER_WALK_D = 3,

	TEX_ACT_NPC1_WALK_L = 4,
	TEX_ACT_NPC1_WALK_U = 5,
	TEX_ACT_NPC1_WALK_R = 6,
	TEX_ACT_NPC1_WALK_D = 7,

	TEX_ACT_NPC2_WALK_L = 8,
	TEX_ACT_NPC2_WALK_U = 9,
	TEX_ACT_NPC2_WALK_R = 10,
	TEX_ACT_NPC2_WALK_D = 11,

	TEX_ACT_NPC3_WALK_L = 12,
	TEX_ACT_NPC3_WALK_U = 13,
	TEX_ACT_NPC3_WALK_R = 14,
	TEX_ACT_NPC3_WALK_D = 15,

	TEX_ACT_NPC4_WALK_L = 16,
	TEX_ACT_NPC4_WALK_U = 17,
	TEX_ACT_NPC4_WALK_R = 18,
	TEX_ACT_NPC4_WALK_D = 19,

	TEX_ACT_NPC5_WALK_L = 20,
	TEX_ACT_NPC5_WALK_U = 21,
	TEX_ACT_NPC5_WALK_R = 22,
	TEX_ACT_NPC5_WALK_D = 23,

	TEX_ACT_NPC6_WALK_L = 24,
	TEX_ACT_NPC6_WALK_U = 25,
	TEX_ACT_NPC6_WALK_R = 26,
	TEX_ACT_NPC6_WALK_D = 27,

	TEX_ACT_NPC7_WALK_L = 28,
	TEX_ACT_NPC7_WALK_U = 29,
	TEX_ACT_NPC7_WALK_R = 30,
	TEX_ACT_NPC7_WALK_D = 31,

	TEX_ACT_NPC8_WALK_L = 32,
	TEX_ACT_NPC8_WALK_U = 33,
	TEX_ACT_NPC8_WALK_R = 34,
	TEX_ACT_NPC8_WALK_D = 35,

	TEX_ACT_NPC9_WALK_L = 36,
	TEX_ACT_NPC9_WALK_U = 37,
	TEX_ACT_NPC9_WALK_R = 38,
	TEX_ACT_NPC9_WALK_D = 39,

	TEX_ACT_NPC10_WALK_L = 40,
	TEX_ACT_NPC10_WALK_U = 41,
	TEX_ACT_NPC10_WALK_R = 42,
	TEX_ACT_NPC10_WALK_D = 43,

	TEX_ACT_NPC11_WALK_L = 44,
	TEX_ACT_NPC11_WALK_U = 45,
	TEX_ACT_NPC11_WALK_R = 46,
	TEX_ACT_NPC11_WALK_D = 47,

	TEX_ACT_NPC12_WALK_L = 48,
	TEX_ACT_NPC12_WALK_U = 49,
	TEX_ACT_NPC12_WALK_R = 50,
	TEX_ACT_NPC12_WALK_D = 51,

	TEX_ACT_NPC13_WALK_L = 52,
	TEX_ACT_NPC13_WALK_U = 53,
	TEX_ACT_NPC13_WALK_R = 54,
	TEX_ACT_NPC13_WALK_D = 55,

	TEX_ACT_NPC14_WALK_L = 56,
	TEX_ACT_NPC14_WALK_U = 57,
	TEX_ACT_NPC14_WALK_R = 58,
	TEX_ACT_NPC14_WALK_D = 59,

	TEX_ACT_NPC15_WALK_L = 60,
	TEX_ACT_NPC15_WALK_U = 61,
	TEX_ACT_NPC15_WALK_R = 62,
	TEX_ACT_NPC15_WALK_D = 63,

	TEX_ACT_TANK_WALK_L = 64,
	TEX_ACT_TANK_WALK_U = 65,
	TEX_ACT_TANK_WALK_R = 66,
	TEX_ACT_TANK_WALK_D = 67,

	TEX_ACT_MAX
};

enum UITextureIndex
{
	TEX_UI_ITEM_0 = 0,
	TEX_UI_ITEM_1 = 1,
	TEX_UI_ITEM_2 = 2,
	TEX_UI_ITEM_3 = 3,
	TEX_UI_ITEM_4 = 4,
	TEX_UI_ITEM_5 = 5,
	TEX_UI_ITEM_6 = 6,
	TEX_UI_ITEM_7 = 7,

	TEX_UI_GUN = 8,
	TEX_UI_CLOTH = 9,
	TEX_UI_SHOE = 10,
	TEX_UI_HAND = 11,
	
	TEX_UI_MAIN = 12,
	TEX_UI_TEXTBOX = 13,
	TEX_UI_CANNON = 14,
	TEX_UI_STATUS = 15,
	TEX_UI_TOP = 16,

	TEX_BATUI_HUM_MAIN = 17,
	TEX_BATUI_ITEMHUM_0 = 18,
	TEX_BATUI_ITEMHUM_1 = 19,
	TEX_BATUI_ITEMHUM_2 = 20,

	TEX_BATUI_TANK_MAIN = 21,
	TEX_BATUI_ITEMTANK_0 = 22,
	TEX_BATUI_ITEMTANK_1 = 23,
	TEX_BATUI_ITEMTANK_2 = 24,
	TEX_BATUI_ITEMTANK_3 = 25,
	TEX_BATUI_INFOBOX = 26,

	TEX_UI_MAX
};

enum BattleTextureIndex
{
	TEX_BAT_ANT = 0,
	TEX_BAT_BUG = 1,
	TEX_BAT_SNAIL = 2,
	TEX_BAT_FIRE = 3,
	TEX_BAT_GUN = 4,
	TEX_BAT_CANNON = 5,
	TEX_BAT_BOSS = 6,
	TEX_BAT_PLAYER = 7,
	TEX_BAT_TANK = 8,
	TEX_BAT_DIE = 9,
	TEX_BAT_PLAYER_GUN = 10,
	TEX_BAT_EFFECT_GUN_1 = 11,
	TEX_BAT_EFFECT_GUN_2 = 12,
	TEX_BAT_PLAYER_SLING = 13,
	TEX_BAT_PLAYER_TOOL = 14,
	TEX_BAT_SLING_SHOT = 15,
	TEX_BAT_BOMB_SHOT = 16,
	TEX_BAT_FIRE_SHOT = 17,
	TEX_BAT_FIRE_SHOT_2 = 18,
	TEX_BAT_CANNON_SHOT = 19,
	TEX_BAT_CANNON_SHOT_2 = 20,
	TEX_BAT_EFFECT_BOMB = 21,
	TEX_BAT_EFFECT_FIRE = 22,
	TEX_BAT_EFFECT_CANNON = 23,

	TEX_BAT_MAX
};

// To Do: refactor
class TextureManager: public Singleton<TextureManager>
{
public:
	TextureManager(void);
	~TextureManager(void);

	void SetFileList(const char* filename, int index);
	bool LoadTextures(void);
	bool LoadSystemTextures(void);
	bool LoadTileTextures(void);
	bool LoadActorTextures(void);
	bool LoadUITextures(void);
	bool LoadBattleTextures(void);

	void ReleaseTextures(void);
	void ReleaseSystemTexures(void);
	void ReleaseTileTexures(void);
	void ReleaseActorTexures(void);
	void ReleaseUITexures(void);
	void ReleaseBattleTexures(void);

	IDirect3DTexture9* GetTexture(int index) { assert(index >= 0 && index < MAX_TEXTURES); return mpTextures[index]; }
	IDirect3DTexture9* GetSystemTex(int index) { assert(index >= 0 && index < TEX_SYS_MAX); return mpSystemTex[index]; }
	IDirect3DTexture9* GetTileTex(int index) { assert(index >= 0 && index < MT_MAX); return mpTileTex[index]; }
	IDirect3DTexture9* GetActorTex(int index) { assert(index >= 0 && index < TEX_ACT_MAX); return mpActorTex[index]; }
	IDirect3DTexture9* GetUITex(int index) { assert(index >= 0 && index < TEX_UI_MAX); return mpUITex[index]; }
	IDirect3DTexture9* GetBattleTex(int index) { assert(index >= 0 && index < TEX_BAT_MAX); return mpBattleTex[index]; }

private:
	char* mpFileList[MAX_TEXTURES];
	IDirect3DTexture9* mpTextures[MAX_TEXTURES];
	IDirect3DTexture9* mpSystemTex[TEX_SYS_MAX];
	IDirect3DTexture9* mpTileTex[MT_MAX];
	IDirect3DTexture9* mpActorTex[TEX_ACT_MAX];
	IDirect3DTexture9* mpUITex[TEX_UI_MAX];
	IDirect3DTexture9* mpBattleTex[TEX_BAT_MAX];
};
#define gTexMgr TextureManager::GetInstance()

#endif