#include "DialogSystemMgr.h"
#include "DialogData.h"

DECLARE_SINGLETON(DialogSystemMgr);
static DialogSystemMgr sDialogSystemMgr;

DialogSystemMgr::DialogSystemMgr(void)
{
}
	
DialogSystemMgr::~DialogSystemMgr(void)
{
}

DialogRecordList DialogSystemMgr::GetDialogRecordList(int actorId)
{
	DialogRecordList recordList;
	for(int i=0; i<MAX_RECORDS; i++)
	{
		if(sDialogRecords[i].actorId == actorId)
		{
			recordList.push_back(sDialogRecords[i]);
		}
	}
	return recordList;
}

DialogRecord* DialogSystemMgr::GetDialogRecordByState(int state)
{
	for(unsigned int i=0; i<mCurRecordList.size(); i++)
	{
		if(mCurRecordList[i].gameState == state)
		{
			return &(mCurRecordList[i]);
		}
	}
	return NULL;
}