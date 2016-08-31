#ifndef _MapRenderObj_H_
#define _MapRenderObj_H_

#include "../../Engine/Hud/Sprite.h"
#include "MapObject.h"

class MapRenderObj: public MapObject
{
public:
	MapRenderObj(void);
	~MapRenderObj(void);

	void Init(int texId, int scaleSize);
	void SetPos(float x, float y);

	void Update(void);
	void Render(void);

	int GetTexID(void) { return mTexId; }
	void SetVisible(bool visible) { mVisible = visible; }
	bool IsVisible(void) { return mVisible; }

protected:
	Sprite mSprRenderObj;
	int mTexId;
	bool mVisible;
};

#endif