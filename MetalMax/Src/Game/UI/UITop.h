#ifndef _UITop_H_
#define _UITop_H_

#include "UIBase.h"
#include "UIMisc.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

enum UIT_Type
{
	UITOP_BAR = 0,
	UITOP_HERO = 1,
	UITOP_INN = 2,
	UITOP_CANNON = 3,
	UITOP_TOOL = 4,
	UITOP_EQUIP = 5,
	UITOP_TOOL_ITEM = 6,
	UITOP_EQUIP_ITEM = 7,
};

class UITop: public UIBase
{
public:
	UITop(void);
	~UITop(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

	void SetUIType(UIT_Type type) { mType = type; }

private:
	void OnItemSelected(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtPlace1;
	Text mTxtPlace2;
	Text mTxtGold;
	Text mTxtLines[MAX_LINES];

	UIT_Type mType;
	int mMaxItemNum;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
enum UIS_Type
{
	UIS_TOOL = 0,
	UIS_EQUIP = 1
};

class UISell: public UIBase
{
public:
	UISell(void);
	~UISell(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

	void SetUISType(UIS_Type type) { mType = type; }

private:
	void OnItemSelected(void);
	void UpdateSellText(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtLines[MAX_ITEMS];
	UIS_Type mType;
	int mSelectRow;
	int mSelectCol;
	int mMaxItemNum;

};

#endif