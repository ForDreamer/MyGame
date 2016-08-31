#ifndef _MapObject_H_
#define _MapObject_H_

class MapObject
{
	friend class MapManager;
public:
	MapObject(void);
	~MapObject(void);

	virtual void SetPos(float x, float y);

	float GetPosX(void) { return mPosX; }
	float GetPosY(void) { return mPosY; }
	int GetTileX(void) { return mTileX; }
	int GetTileY(void) { return mTileY; }

protected:
	virtual void Update(void) {}
	virtual void Render(void) {}

protected:
	float mPosX;
	float mPosY;
	int mTileX;
	int mTileY;
};

#endif