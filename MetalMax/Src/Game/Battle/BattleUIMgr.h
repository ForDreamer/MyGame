#ifndef _BattleUIMgr_H_
#define _BattleUIMgr_H_

#include "../../Engine/Misc/Singleton.h"
#include "../UI/UIBase.h"

enum BattleUIType
{
	BATUI_HUMAN_MAIN = 0,
	BATUI_TANK_MAIN = 1,
	BATUI_ATK_LIST = 2,
	BATUI_ASSIST_LIST = 3,
	BATUI_TOOL_LIST = 4,
	BATUI_TEXTBOX = 5,
	BATUI_INFOBOX = 6,

	BATUI_MAX
};

class BattleUIMgr: public Singleton<BattleUIMgr>
{
public:
	BattleUIMgr(void);
	~BattleUIMgr(void);
	
	void Update(void);
	void Render(void);

	void InitUIElements(void);
	void ReleaseUIElements(void);
	void CloseAllUI(void);

	UIBase* GetUI(BattleUIType type) { return mpUIelems[type]; }

private:
	UIBase* mpUIelems[BATUI_MAX];
};
#define gBattleUIMgr BattleUIMgr::GetInstance()

#endif