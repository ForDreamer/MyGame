#ifndef _BatUIInfoBox_H_
#define _BatUIInfoBox_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

#define MAX_INFO_LINES 4
#define MAX_INFO_LENGTH 32

class BatUIInfoBox: public UIBase
{
public:
	BatUIInfoBox(void);
	~BatUIInfoBox(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);
	void SetOnFocus(bool focus);

	void SetLineText(int line, const char* text);

private:
	void OnItemSelected(void);
	void ClearText();

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtLines[MAX_INFO_LINES];
	char mTxtStrings[MAX_INFO_LINES][MAX_INFO_LENGTH];
	bool mbShowCursor;
};

#endif