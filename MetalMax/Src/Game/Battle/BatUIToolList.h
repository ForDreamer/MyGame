#ifndef _BatUIToolList_H_
#define _BatUIToolList_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

#define MAX_ITEMS 8

class BatUIToolList: public UIBase
{
public:
	BatUIToolList(void);
	~BatUIToolList(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

private:
	void OnItemSelected(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtLines[MAX_ITEMS];
	int mSelectRow;
	int mSelectCol;
	int mMaxItemNum;
};

#endif