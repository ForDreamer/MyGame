#ifndef _BatUIAssistList_H_
#define _BatUIAssistList_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

#define MAX_SELECT_ASSIST_NUM 3

class BatUIAssistList: public UIBase
{
	enum
	{
		AST_LOAD = 0,
		AST_GUARD = 1,
		AST_ESCAPE = 2
	};

public:
	BatUIAssistList(void);
	~BatUIAssistList(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);
	void SetOnFocus(bool focus);

private:
	void OnItemSelected(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtEnemys[MAX_SELECT_ASSIST_NUM];
};

#endif