#ifndef _BatUITextBox_H_
#define _BatUITextBox_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

#define MAX_TEXT_LINES 4
#define MAX_TEXT_LENGTH 32

class BatUITextBox: public UIBase
{
public:
	BatUITextBox(void);
	~BatUITextBox(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);
	void SetOnFocus(bool focus);

	void ShowSelector(void) { mSprCursor.SetVisible(true); }
	void HideSelector(void) { mSprCursor.SetVisible(false); }

	void SetLineText(int line, const char* text);
	void SetIsOnLoad(bool onload) { mbIsOnLoad = onload; }

private:
	void OnItemSelected(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtLines[MAX_TEXT_LINES];
	char mTxtStrings[MAX_TEXT_LINES][MAX_TEXT_LENGTH];

	bool mbIsOnLoad;
};

#endif