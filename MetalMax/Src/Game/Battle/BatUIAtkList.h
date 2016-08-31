#ifndef _BatUIAtkList_H_
#define _BatUIAtkList_H_

#include "../UI/UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

#define MAX_SELECT_NUM 4

class BatUIAtkList: public UIBase
{
public:
	BatUIAtkList(void);
	~BatUIAtkList(void);

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
	Text mTxtEnemys[MAX_SELECT_NUM];

	int mMaxSelectNum;
};

#endif