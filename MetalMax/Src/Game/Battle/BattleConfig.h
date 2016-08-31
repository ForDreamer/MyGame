#ifndef _BattleConfig_H_
#define _BattleConfig_H_

#include "../../Engine/Manager/TextureManager.h"

#define MAX_ENEMY_CONFIGS 6
#define MAX_ENEMY_NUM 4

// To Do: data driven
struct EnemyConfig
{
	int texId;
	float posX;
	float posY;
	float sizeX;
	float sizeY;
	char nameFix;
};

static const int sEnemyNum_World[MAX_ENEMY_CONFIGS] = {2, 3, 4, 2, 1, 1};
static const int sEnemyNum_Cave2[MAX_ENEMY_CONFIGS] = {3, 4, 2, 3, 2, 1};
static const int sEnemyNum_Cave3[MAX_ENEMY_CONFIGS] = {1, 1, 2, 2, 4, 2};

static EnemyConfig sEnemyConfigs_World[MAX_ENEMY_CONFIGS][MAX_ENEMY_NUM] =
{
	{
		{TEX_BAT_ANT, -220.0f, 80.0f, 72.0f, 60.0f, 'A'},
		{TEX_BAT_ANT, -140.0f, 80.0f, 72.0f, 60.0f, 'B'}
	},
	{
		{TEX_BAT_ANT, -240.0f, 80.0f, 72.0f, 60.0f, 'A'},
		{TEX_BAT_ANT, -160.0f, 80.0f, 72.0f, 60.0f, 'B'},
		{TEX_BAT_ANT, -80.0f, 80.0f, 72.0f, 60.0f, 'C'}
	},
	{
		{TEX_BAT_ANT, -240.0f, 40.0f, 72.0f, 60.0f, 'A'},
		{TEX_BAT_ANT, -160.0f, 40.0f, 72.0f, 60.0f, 'B'},
		{TEX_BAT_ANT, -80.0f, 40.0f, 72.0f, 60.0f, 'C'},
		{TEX_BAT_SNAIL, -240.0f, 130.0f, 72.0f, 30.0f, '\0'}
	},
	{
		{TEX_BAT_BUG, -220.0f, 80.0f, 144.0f, 60.0f, 'A'},
		{TEX_BAT_BUG, -120.0f, 80.0f, 144.0f, 60.0f, 'B'},
	},
	{
		{TEX_BAT_SNAIL, -220.0f, 80.0f, 72.0f, 30.0f, '\0'}
	},
	{
		{TEX_BAT_FIRE, -220.0f, 80.0f, 72.0f, 60.0f, '\0'}
	},
};

static EnemyConfig sEnemyConfigs_Cave2[MAX_ENEMY_CONFIGS][MAX_ENEMY_NUM] =
{
	{
		{TEX_BAT_BUG, -220.0f, 130.0f, 144.0f, 60.0f, 'A'},
		{TEX_BAT_BUG, -120.0f, 130.0f, 144.0f, 60.0f, 'B'},
		{TEX_BAT_BUG, -220.0f, 40.0f, 144.0f, 60.0f, 'C'}
	},
	{
		{TEX_BAT_ANT, -220.0f, 130.0f, 72.0f, 60.0f, 'A'},
		{TEX_BAT_ANT, -140.0f, 130.0f, 72.0f, 60.0f, 'B'},
		{TEX_BAT_ANT, -220.0f, 40.0f, 72.0f, 60.0f, 'C'},
		{TEX_BAT_ANT, -140.0f, 40.0f, 72.0f, 60.0f, 'D'},
	},
	{
		{TEX_BAT_FIRE, -220.0f, 130.0f, 72.0f, 60.0f, '\0'},
		{TEX_BAT_SNAIL, -220.0f, 40.0f, 72.0f, 30.0f, '\0'}
	},
	{
		{TEX_BAT_FIRE, -220.0f, 130.0f, 72.0f, 60.0f, '\0'},
		{TEX_BAT_BUG, -220.0f, 40.0f, 144.0f, 60.0f, 'A'},
		{TEX_BAT_BUG, -120.0f, 40.0f, 144.0f, 60.0f, 'B'}
	},
	{
		{TEX_BAT_BUG, -220.0f, 80.0f, 144.0f, 60.0f, 'A'},
		{TEX_BAT_BUG, -120.0f, 80.0f, 144.0f, 60.0f, 'B'}
	},
	{
		{TEX_BAT_FIRE, -220.0f, 80.0f, 72.0f, 60.0f, '\0'},
	},
};

static EnemyConfig sEnemyConfigs_Cave3[MAX_ENEMY_CONFIGS][MAX_ENEMY_NUM] =
{
	{
		{TEX_BAT_CANNON, -220.0f, 80.0f, 144.0f, 60.0f, '\0'},
	},
	{
		{TEX_BAT_GUN, -220.0f, 85.0f, 144.0f, 60.0f, '\0'},
	},
	{
		{TEX_BAT_GUN, -220.0f, 130.0f, 144.0f, 60.0f, 'A'},
		{TEX_BAT_GUN, -220.0f, 45.0f, 144.0f, 60.0f, 'B'}
	},
	{
		{TEX_BAT_FIRE, -220.0f, 130.0f, 72.0f, 60.0f, 'A'},
		{TEX_BAT_FIRE, -220.0f, 40.0f, 72.0f, 60.0f, 'B'}
	},
	{
		{TEX_BAT_BUG, -220.0f, 130.0f, 144.0f, 60.0f, 'A'},
		{TEX_BAT_BUG, -120.0f, 130.0f, 144.0f, 60.0f, 'B'},
		{TEX_BAT_BUG, -220.0f, 40.0f, 144.0f, 60.0f, 'C'},
		{TEX_BAT_BUG, -120.0f, 40.0f, 144.0f, 60.0f, 'D'}
	},
	{
		{TEX_BAT_CANNON, -220.0f, 130.0f, 144.0f, 60.0f, '\0'},
		{TEX_BAT_GUN, -220.0f, 45.0f, 144.0f, 60.0f, '\0'}
	},
};

#endif