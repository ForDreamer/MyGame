#ifndef _MapNode_H_
#define _MapNode_H_

#include "../../Engine/Hud/Sprite.h"
#include "MapTexDef.h"
#include "MapObject.h"

class MapNode: public MapObject
{
public:
	MapNode(void);
	~MapNode(void);

	void Init(int texID, bool actorMovable, bool tankMovable);
	void ReInit(int texID, bool actorMovable, bool tankMovable);
	void SetPos(float x, float y);

	void Update(void);
	void Render(void);

	bool IsActorMovable(void) { return mbActorMovable; }
	bool IsTankMovable(void) { return mbTankMovable; }
	MapTexID GetTexID(void) { return mTexId; }

protected:
	MapTexID mTexId;
	Sprite mSprTile;

	bool mbActorMovable;
	bool mbTankMovable;
};

#endif