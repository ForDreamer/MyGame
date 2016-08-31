#ifndef _UIMisc_H_
#define _UIMisc_H_

#include "UIBase.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"
#include "../MapEngine/MapActor.h"

#define MAX_LINES 4
#define MAX_ITEMS 8
#define TXT_BUFFER 32

class UITextBox: public UIBase
{
public:
	enum SelectorID
	{
		SELECT_YES = 0,
		SELECT_NO = 1
	};

public:
	UITextBox(void);
	~UITextBox(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

	void SetText(const char* text);
	void SetTalkingActor(MapActor* pActor) { mpTalkingActor = pActor; }
	void SetIntoInnScene(void) { mbIntoInnScene = true; }
	void SetIntoWanterScene(void) { mbIntoWanterScene = true; }

private:
	void ClearTxtBuffer(void);
	void UpdateVisibleLines(void);
	void SetTxtYesNoVisible(bool visible);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtLines[MAX_LINES];
	char mStrTxts[MAX_LINES][TXT_BUFFER];
	int mCurLine;
	int mCurSegment;
	int mTotalLine;
	int mTotalSegment;
	int mSegTargets[MAX_LINES];

	Text mTxtYes;
	Text mTxtNo;
	Sprite mSprSelector;
	SelectorID mSelectYesNo;

	MapActor* mpTalkingActor;
	//To Do: refactor switch scene 
	bool mbIntoInnScene;
	bool mbIntoWanterScene;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class UISelector: public UIBase
{
public:
	enum SelectorUser
	{
		UIS_MISC = 0,
		UIS_TOOL = 1
	};

public:
	UISelector(void);
	~UISelector(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

	void SetUser(SelectorUser user) { mUser = user; }
	void SetMaxItemNum(int max) { assert(max <= MAX_LINES); mMaxItemNum = max; }
	void SetLineText(int line, const char* text) { assert(line < MAX_LINES); mTxtLines[line].SetText(text); }

private:
	void OnItemSelected(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	SelectorUser mUser;
	Text mTxtLines[MAX_LINES];
	int mMaxItemNum;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class UIBattleInfo: public UIBase
{
public:
	UIBattleInfo(void);
	~UIBattleInfo(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

private:
	Sprite mSprBG;
	Text mTxtLines[MAX_LINES];
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class UITools: public UIBase
{
public:
	UITools(void);
	~UITools(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

private:
	void ApplyUseItemEffect(UITextBox* pUITextbox);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Text mTxtLines[MAX_ITEMS];
	int mSelectRow;
	int mSelectCol;
	int mMaxItemNum;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class UICannon: public UIBase
{
public:
	UICannon(void);
	~UICannon(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

private:
	Sprite mSprBG;
	Text mTxtMain;
	Text mTxtSpecial;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class UIEquip: public UIBase
{
public:
	UIEquip(void);
	~UIEquip(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

private:
	void UpdateStatusText(void);

private:
	Sprite mSprBG;
	Sprite mSprCursor;
	Sprite mSprEquips[4];
	int mEquipsPos[4];
	Text mTxtLines[MAX_ITEMS];
	Text mTxtLV;
	Text mTxtAT;
	Text mTxtDF;
	int mSelectRow;
	int mSelectCol;
	int mMaxItemNum;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class UIStatus: public UIBase
{
public:
	UIStatus(void);
	~UIStatus(void);

	void Init(void);
	void Update(float baseX, float baseY);
	void Render(void);

	void OpenUI(void);
	void CloseUI(void);

private:
	Sprite mSprBG;
	Sprite mSprPlayer;
	Sprite mSprEquips[4];
	int mEquipsPos[4];

	Text mTxtName;
	Text mTxtLV;
	Text mTxtHP;

	Text mTxtAT;
	Text mTxtDF;
	Text mTxtSTR;
	Text mTxtINT;
	Text mTxtAG;
	Text mTxtPH;
	Text mTxtEXP;
	Text mTxtGOL;

	Text mTxtLines[MAX_ITEMS];
};

#endif