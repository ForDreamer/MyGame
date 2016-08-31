#ifndef _BatUITankMain_H_
#define _BatUITankMain_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

enum BatUITankMainItems
{
	BAT_UIT_ATK = 0,
	BAT_UIT_ATK_SUB = 1,
	BAT_UIT_SPECIAL = 2,
	BAT_UIT_ASSIST = 3,

	BAT_UIT_MAX
};

#define MAX_CHAR_INFO_NUM 16

class BatUITankMain: public UIBase
{
public:
	BatUITankMain(void);
	~BatUITankMain(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);
	void SetOnFocus(bool focus);

	void ShowSelector(void) { mSprCursor.SetVisible(true); }
	void HideSelector(void) { mSprCursor.SetVisible(false); }

	void SetInfoBarText(const char* text)
	{
		assert(strlen(text) < MAX_CHAR_INFO_NUM);
		strcpy_s(mStrInfoBar, text);
		mTxtInfoBar.SetText(mStrInfoBar);
	}

private:
	void OnItemSelected(void);
	void ClearSprHints(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Sprite mSprHints[BAT_UIT_MAX];
	Text mTxtInfoBar;
	char mStrInfoBar[MAX_CHAR_INFO_NUM];
	Text mTxtPlayerName;
	Text mTxtPlayerHp;
	Text mTxtTankName;
	Text mTxtTankSp;
};

#endif