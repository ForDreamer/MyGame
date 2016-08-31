#include "MapTrigger.h"

MapTrigger::MapTrigger(void)
:	mType(TYPE_ENTRY),
	mEntryInfo(0, 0, 0)
{
}

MapTrigger::~MapTrigger(void)
{
}

void MapTrigger::Init(int type, EntryInfo entryInfo)
{
	mType = TriggerType(type);
	mEntryInfo = entryInfo;
}
