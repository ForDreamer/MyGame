#ifndef _MapTrigger_H_
#define _MapTrigger_H_

#include "MapObject.h"

enum TriggerType
{
	TYPE_ENTRY = 0,
	TYPE_ENTER_DOOR = 1,
	TYPE_LEFT_DOOR = 2
};

struct EntryInfo
{
	EntryInfo(int mapId, short tileX, short tileY)
	{
		targetMapId = mapId;
		targetTileX = tileX;
		targetTileY = tileY;
	}
	int targetMapId;
	short targetTileX;
	short targetTileY;
};

class MapTrigger: public MapObject
{
public:
	MapTrigger(void);
	~MapTrigger(void);

	void Init(int type, EntryInfo entryInfo);
	TriggerType GetType(void) { return mType; }
	const EntryInfo& GetEntryInfo(void) { return mEntryInfo; }

protected:
	TriggerType mType;
	EntryInfo mEntryInfo;
};

#endif