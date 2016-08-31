#include "BattleActor.h"
#include "BattleManager.h"
#include "../Manager/CharacterMgr.h"
#include "../Manager/BattleTankMgr.h"
#include "../Manager/EquipMgr.h"
#include "../../Main/Global.h"
#include "../../Engine/Manager/TextureManager.h"

static const BatActorAttrib sBatActorAttribs[MAX_ENEMY_TYPE] =
{
	{"æﬁ“œ", 7, 7, 30, 5, 18, 1, 2, WT_BODY},
	{"…±»À≥Ê", 10, 10, 35, 10, 22, 2, 3, WT_BODY},
	{"∑¬…˙Œœ≈£", 15, 15, 32, 12, 10, 3, 5, WT_GUN},
	{"ª—Ê«π", 20, 20, 39, 8, 18, 3, 5, WT_FIRE},
	{"…˙ŒÔ≈⁄", 35, 35, 45, 15, 30, 5, 10, WT_GUN},
	{"º”≈©≈⁄", 50, 50, 60, 20, 25, 10, 20, WT_CANNON},
	{"’Ωπ∑", 260, 260, 72, 25, 25, 50, 100, WT_CANNON}
};


BattleActor::BattleActor(void)
{
}
	
BattleActor::~BattleActor(void)
{
}

void BattleActor::Init(int texId, int enemyId, float posX, float posY, float sizeX, float sizeY)
{
	mTexId = texId;
	mEnemyId = enemyId;

	mSprActor.SetPosition(posX, posY);
	mSprActor.SetSize(sizeX, sizeY);
	mSprActor.SetTexture(gTexMgr.GetBattleTex(texId));
	mSprActor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	for(int i=0; i<MAX_BATACTOR_NAME; i++)
		mAttrib.Name[i] = '\0';
	
	mbIsGuarding = false;
	mbDieBlink = false;
	mbBlinkTimer = 0.0f;
	mbAttackBlink = false;
	mbAttackBlinkTimer = 0.0f;
	mbAttackBlinkCount = 0;
	mbDamaged = false;
	mbDamageTimer = 0.0f;

	mSP = 0;
	mSPMax = 0;
}

void BattleActor::Update(void)
{
	if(mbDieBlink)
	{
		mbBlinkTimer += Global::GetTimeDelta();
		if(mbBlinkTimer > 0.02f)
		{
			mbBlinkTimer = 0.0f;
			mSprActor.SetVisible(!mSprActor.GetVisible());
		}
	}

	if(mbAttackBlink)
	{
		mbAttackBlinkTimer += Global::GetTimeDelta();
		if(mbAttackBlinkTimer > 0.02f)
		{
			mbAttackBlinkTimer = 0.0f;

			mbAttackBlinkCount ++;
			if(mbAttackBlinkCount >= 2)
				mbAttackBlinkCount = 0;
			if(mbAttackBlinkCount == 0)
				mSprActor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
			else
				mSprActor.SetBlendMode(Sprite::BLENDMODE_DEST_ONE);
		}
	}

	if(mbDamaged)
	{
		mbDamageTimer += Global::GetTimeDelta();
		if(mbDamageTimer > 0.2f)
		{
			mbDamageTimer = 0.0f;
			mbDamaged = false;
		}
	}

	mSprActor.Update();
}

void BattleActor::Render(void)
{
	if(mbDamaged)
		mSprActor.SetPositionX(mSprActor.GetPositionX() + 7.5f);

	mSprActor.Render();
	if(mbAttackBlink && mbAttackBlinkCount == 1)
	{
		mSprActor.Render();
	}

	if(mbDamaged)
		mSprActor.SetPositionX(mSprActor.GetPositionX() - 7.5f);
}

void BattleActor::SetAttackBlink(bool blink) 
{
	mbAttackBlink = blink;
	mbAttackBlinkTimer = 0.0f;
	mbAttackBlinkCount = 0;
	mSprActor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
}

void BattleActor::SetBeenDamaged(bool damaged)
{
	if(IsPlayer())
	{
		mbDamaged = damaged;
		mbDamageTimer = 0.0f;
	}
}


void BattleActor::SetupBatAttribs(int enemyId, char nameFix)
{
	assert(enemyId < MAX_ENEMY_TYPE);

	if(enemyId == -1)
	{
		Character* pPlayer = gCharacterMgr.GetCharacter(CHARID_PLAYER);
		strcpy_s(mAttrib.Name, pPlayer->GetName());

		mAttrib.HP = pPlayer->GetAttrib(CAB_HP);
		mAttrib.HPMax = pPlayer->GetAttrib(CAB_HPMAX);
		mAttrib.AT = pPlayer->GetAttrib(CAB_AT);
		mAttrib.DF = pPlayer->GetAttrib(CAB_DF);
		mAttrib.Speed = pPlayer->GetAttrib(CAB_AGL);
		mAttrib.Exp = 0;
		mAttrib.Gold = 0;

		BattleTank* pTank = gBattleTankMgr.GetTank(TANK_FIRST);
		mSP = pTank->GetSP();
		mSPMax = pTank->GetSPMax();
	}
	else if(enemyId >= 0)
	{
		mAttrib = sBatActorAttribs[enemyId];
		if(nameFix != '\0')
		{
			int lastChar = (int)strlen(mAttrib.Name);
			mAttrib.Name[lastChar] = ' ';
			mAttrib.Name[lastChar+1] = nameFix;
			mAttrib.Name[lastChar+2] = '\0';
		}
	}
}

void BattleActor::RefreshTexIdByMode(void)
{
	BattleMode mode = gBattleMgr.GetBattleMode();
	if(mode	== BM_HUMAN)
	{
		mSprActor.SetTexture(gTexMgr.GetBattleTex(TEX_BAT_PLAYER));
		mSprActor.SetPositionY(mSprActor.GetPositionY() - 5.0f);
	}
	else if(mode == BM_TANK)
	{
		mSprActor.SetTexture(gTexMgr.GetBattleTex(TEX_BAT_TANK));
		mSprActor.SetPositionY(mSprActor.GetPositionY() + 5.0f);
	}
}

void BattleActor::SetSprTexture(int texId)
{
	mSprActor.SetTexture(gTexMgr.GetBattleTex(texId));
}

void BattleActor::ResetSprTexture(void)
{
	if(IsPlayer())
		mSprActor.SetTexture(gTexMgr.GetBattleTex(TEX_BAT_PLAYER));
}

WeaponType BattleActor::GetWeaponTypeByEquip(void)
{
	EquipID equipId = gEquipMgr.GetAttackWeapon();
	if(equipId == EQUIP_SHORTGUN)
		return WT_GUN;
	else if(equipId == EQUIP_SLING)
		return WT_SLING;
	else
		return WT_BODY;
}