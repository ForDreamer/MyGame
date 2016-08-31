#ifndef _DialogSystemMgr_H_
#define _DialogSystemMgr_H_

#include "../../Engine/Misc/Singleton.h"
#include <vector>

using namespace std;

struct DialogRecord
{
	int actorId;
	int dialogId;
	int gameState;
	const char* text;
	int yesId;
	int noId;
};

typedef vector<DialogRecord> DialogRecordList;
class DialogSystemMgr: public Singleton<DialogSystemMgr>
{
public:
	DialogSystemMgr(void);
	~DialogSystemMgr(void);

	DialogRecordList GetDialogRecordList(int actorId);
	DialogRecord* GetDialogRecordByState(int state);

	void SetCurrentRecord(DialogRecord record) { mCurRecord = record; }
	const DialogRecord& GetCurrentRecord(void) { return mCurRecord; }

	void SetCurrentRecordList(DialogRecordList recordList) { mCurRecordList = recordList; }
	const DialogRecordList& GetCurrentRecordList(void) { return mCurRecordList; }

private:
	DialogRecordList mCurRecordList;
	DialogRecord mCurRecord;
};
#define gDialogSystemMgr DialogSystemMgr::GetInstance()

#endif