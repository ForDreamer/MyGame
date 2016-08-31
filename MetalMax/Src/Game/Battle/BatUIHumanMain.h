#ifndef _BatUIHumanMain_H_
#define _BatUIHumanMain_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

enum BatUIHumMainItems
{
	BAT_UIM_ATTACK = 0,
	BAT_UIM_TOOL = 1,
	BAT_UIM_ASSIST = 2,

	BAT_UIM_MAX
};

#define MAX_CHAR_INFO_NUM 16

class BatUIHumanMain: public UIBase
{
public:
	BatUIHumanMain(void);
	~BatUIHumanMain(void);

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
	Sprite mSprHints[BAT_UIM_MAX];
	Text mTxtInfoBar;
	char mStrInfoBar[MAX_CHAR_INFO_NUM];
	Text mTxtPlayerName;
	Text mTxtPlayerHp;
};

#endif