#ifndef _MapActor_H_
#define _MapActor_H_

#include "../../Engine/Hud/Sprite.h"
#include "MapObject.h"

class MapActor: public MapObject
{
public:
	enum MoveDir
	{
		MOVE_DIR_LEFT = 0,
		MOVE_DIR_UP = 1,
		MOVE_DIR_RIGHT = 2,
		MOVE_DIR_DOWN = 3
	};

	enum AIMode
	{
		AI_NONE = 0,
		AI_STAND = 1,
		AI_STEPPING = 2,
		AI_IDLE = 3
	};

public:
	MapActor(void);
	~MapActor(void);

	void Init(int NPCid, int texBaseId, int AImode, int curDir, bool isPlayer=false);
	void SetPos(float x, float y);

	void Update(void);
	void Render(void);

	void SetMoving(MoveDir dir, bool isMove);
	void SetMoveTarget(float tx, float ty) { mMovTargetX = tx; mMovTargetY = ty; }
	void SetMoveTargetTile(int ttx, int tty) { mMoveTargetTileX = ttx; mMoveTargetTileY = tty; }
	int GetMoveTargetTileX(void) { return mMoveTargetTileX; }
	int GetMoveTargetTileY(void) { return mMoveTargetTileY; }
	bool IsMoving(void) { return mbIsMoving; }
	MoveDir GetCurDir(void) { return mCurDir; }
	int GetActorID(void) { return mActorId; }
	AIMode GetAIMode(void) { return mAImode; }

	void SetTalkingDir(MoveDir dir) { sBackupDir = mCurDir; mCurDir = dir; ReInitTalkingSpr(); }
	void RestoreTalkingDir(void) { mCurDir = sBackupDir; ReInitTalkingSpr(); }
	void ReInitTalkingSpr(void);

	void SetVisible(bool visible) { mSprActor.SetVisible(visible); }
	bool IsVisible(void) { return mSprActor.GetVisible(); }
	void SetIsPlayer(bool isPlayer)
	{
		if(isPlayer)
		{
			mbIsPlayer = true;
			mAImode = AI_NONE;
		}
		else
		{
			mbIsPlayer = false;
			mAImode = AI_STAND;
		}
	}
	bool IsPlayer(void) { return mbIsPlayer; }

private:
	void UpdateMoving(void);
	void UpdateAIMoving(void);

protected:
	Sprite mSprActor;
	int mTexIdBase;
	bool mbIsPlayer;

	MoveDir mCurDir;
	bool mbIsMoving;
	float mMovSpeed;
	float mMovTargetX;
	float mMovTargetY;
	int mMoveTargetTileX;
	int mMoveTargetTileY;

	int mActorId;
	AIMode mAImode;

	float mAIThinkTimeLimit;
	float mAIThinkTimeCur;
	MoveDir mAInextDir;

	static MoveDir sBackupDir;
};

#endif