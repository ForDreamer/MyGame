#ifndef _UIMain_H_
#define _UIMain_H_

#include "UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

enum UIMainItems
{
	UIM_TALK = 0,
	UIM_LOAD = 1,
	UIM_POWER = 2,
	UIM_TOOL = 3,
	UIM_EQUIP = 4,
	UIM_CANNON = 5,
	UIM_DETECT = 6,
	UIM_MISC = 7
};

#define MAX_UIM_ITEMS 8
class UIMain: public UIBase
{
public:
	UIMain(void);
	~UIMain(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);
	void SetOnFocus(bool focus);

private:
	void OnItemSelected(void);
	void ClearSprHints(void);
	void ProcessOnTalk(void);
	void ProcessOnDetect(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Sprite mSprHint[MAX_UIM_ITEMS];
	Text mTxtName;
	Text mTxtTankName;
	Text mTxtPlayerInfo;
	Text mTxtTankInfo;
	int mSelectRow;
	int mSelectCol;
};

#endif