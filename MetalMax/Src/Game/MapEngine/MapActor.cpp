#include "MapActor.h"
#include "MapManager.h"
#include "../../Engine/Manager/TextureManager.h"
#include "../../Main/Global.h"

MapActor::MoveDir MapActor::sBackupDir = MapActor::MOVE_DIR_LEFT;

MapActor::MapActor(void)
:	mTexIdBase(0),
	mbIsPlayer(false),
	mCurDir(MOVE_DIR_LEFT),
	mbIsMoving(false),
	mMovSpeed(0),
	mMovTargetX(-1000.0f),
	mMovTargetY(-1000.0f),
	mMoveTargetTileX(-1),
	mMoveTargetTileY(-1),
	mActorId(0),
	mAImode(AI_NONE),
	mAIThinkTimeLimit(0),
	mAIThinkTimeCur(0),
	mAInextDir(MOVE_DIR_LEFT)
{
}

MapActor::~MapActor(void)
{
}

void MapActor::Init(int NPCid, int texBaseId, int AImode, int curDir, bool isPlayer)
{
	mTexIdBase = texBaseId;
	mCurDir = (MoveDir)curDir;
	mbIsPlayer = isPlayer;
	if(mbIsPlayer)
	{
		mMovSpeed = 135.0f;
		mActorId = 0;
		mAImode = AI_NONE;
	}
	else
	{
		mMovSpeed = 70.0f;
		mActorId = NPCid;
		mAImode = (AIMode)AImode;
	}

	mSprActor.SetSize(36.0f, 30.0f);
	mSprActor.SetAnimator(2, 1, 2, 1, 10.0f);
	if(mbIsPlayer)
	{
		mSprActor.StopAnim();
	}
	else
	{
		mSprActor.SetAnimator(2, 1, 2, 1, 4.0f);
		if(mAImode == AI_STEPPING)
			mSprActor.PlayAnim();
		else
			mSprActor.StopAnim();
	}
	mSprActor.SetTexture(gTexMgr.GetActorTex(mTexIdBase + (int)mCurDir));
	mSprActor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);

	if(mAImode == AI_IDLE)
	{
		mAInextDir = mCurDir;
	}

	if(NPCid == TANK_ID)
	{
		mSprActor.SetAnimator(2, 1, 2, 1, 10.0f);
		mMovSpeed = 135.0f;
	}
}

void MapActor::ReInitTalkingSpr(void)
{
	mSprActor.SetTexture(gTexMgr.GetActorTex(mTexIdBase + (int)mCurDir));
	mSprActor.SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
}

void MapActor::SetPos(float x, float y)
{
	MapObject::SetPos(x, y);
	mSprActor.SetPosition(x, y);
}

void MapActor::Update(void)
{
	mSprActor.Update();

	if(mAImode == AI_IDLE)
	{
		UpdateAIMoving();
	}

	if(mAImode == AI_NONE || mAImode == AI_IDLE)
	{
		UpdateMoving();
	}
}

void MapActor::Render(void)
{
	mSprActor.Render();
}


void MapActor::UpdateMoving(void)
{
	if(mbIsMoving)
	{
		float newPos = 0;
		bool bMovDone = false;
		float tempSpeed = 1.0f;

		switch(mCurDir)
		{
			case MOVE_DIR_LEFT:
				newPos = mPosX - mMovSpeed*Global::GetTimeDelta() * tempSpeed;
				if(newPos < mMovTargetX)
				{
					newPos = mMovTargetX;
					bMovDone = true;
				}
				SetPos(newPos, mPosY);
				break;
			case MOVE_DIR_UP:
				newPos = mPosY + mMovSpeed*Global::GetTimeDelta()*0.83f * tempSpeed;
				if(newPos > mMovTargetY)
				{
					newPos = mMovTargetY;
					bMovDone = true;
				}
				SetPos(mPosX, newPos);
				break;
			case MOVE_DIR_RIGHT:
				newPos = mPosX + mMovSpeed*Global::GetTimeDelta() * tempSpeed;
				if(newPos > mMovTargetX)
				{
					newPos = mMovTargetX;
					bMovDone = true;
				}
				SetPos(newPos, mPosY);
				break;
			case MOVE_DIR_DOWN:
				newPos = mPosY - mMovSpeed*Global::GetTimeDelta()*0.83f * tempSpeed;
				if(newPos < mMovTargetY)
				{
					newPos = mMovTargetY;
					bMovDone = true;
				}
				SetPos(mPosX, newPos);
				break;
		}

		if(bMovDone)
		{
			mbIsMoving = false;
			mSprActor.ResetAnim();
			mSprActor.StopAnim();

			bool bInTrigger = false;
			if(mbIsPlayer)
				bInTrigger = gMapMgr.TestTriggers(this);
			if(mActorId == 0 && !bInTrigger)
				gMapMgr.AddMoveStep();
		}
	}
}

void MapActor::SetMoving(MoveDir dir, bool isMove)
{
	mCurDir = dir;
	mbIsMoving = isMove;
	mSprActor.SetTexture(gTexMgr.GetActorTex(mTexIdBase + (int)mCurDir));
	mSprActor.ResetAnim();

	if(mbIsMoving)
		mSprActor.PlayAnim();
	else
		mSprActor.StopAnim();
}

void MapActor::UpdateAIMoving(void)
{
	if(!mbIsMoving)
	{
		mAIThinkTimeCur += Global::GetTimeDelta();
		if(mAIThinkTimeCur > mAIThinkTimeLimit)
		{
			mAIThinkTimeCur = 0;
			mAIThinkTimeLimit = (float)(rand()%2000 - 1000) / 1000.0f;
			mAInextDir = MoveDir(rand()%4);

			int moveTargetX = mTileX;
			int moveTargetY = mTileY;
			switch(mAInextDir)
			{
				case MOVE_DIR_LEFT: moveTargetX--; break;
				case MOVE_DIR_UP: moveTargetY--; break;
				case MOVE_DIR_RIGHT: moveTargetX++; break;
				case MOVE_DIR_DOWN: moveTargetY++; break;
			}
			gMapMgr.MoveActor(mActorId, moveTargetX, moveTargetY, mAInextDir);
		}
	}
}