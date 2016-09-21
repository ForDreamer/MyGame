#ifndef _MapDetector_H_
#define _MapDetector_H_

#include "MapObject.h"
#include "../../Engine/Hud/Sprite.h"

enum DetectorType
{//调查（通缉，电脑，售货机，钱，道具，装备，洞穴）
	DT_WANTER = 0,
	DT_COMPUTER = 1,
	DT_SELLER = 2,
	DT_GOLD = 3,
	DT_ITEM = 4,
	DT_EQUIP = 5,
	DT_CAVE = 6
};

class MapDetector: public MapObject
{
public:
	MapDetector(void);
	~MapDetector(void);

	void Init(int type, int unlockId);
	DetectorType GetType(void) { return mType; }

	void SetPos(float x, float y);
	void Update(void);
	void Render(void);

	void SetItemBoxUnlock(void);
	bool IsItemBoxUnlocked(void);

protected:
	DetectorType mType;
	Sprite mSprItem;
	int mUnlockId;
};

#endif