#ifndef _UIManager_H_
#define _UIManager_H_

#include "../../Engine/Misc/Singleton.h"
#include "UIBase.h"

enum UIType
{
	UIT_MAIN = 0,
	UIT_TEXTBOX = 1,
	UIT_SELECTOR = 2,
	UIT_BATTLEINFO = 3,
	UIT_TOOLS = 4,
	UIT_CANNON = 5,
	UIT_EQUIP = 6,
	UIT_STATUS = 7,
	UIT_TOP = 8,
	UIT_SELL = 9,

	UIT_MAX
};

class UIManager: public Singleton<UIManager>
{
public:
	UIManager(void);
	~UIManager(void);
	
	void Update(void);
	void Render(void);

	void InitUIElements(void);
	void ReleaseUIElements(void);
	void CloseAllUI(void);

	UIBase* GetUI(UIType type) { return mpUIelems[type]; }
	void SetActive(bool active) { mbActive = active; }
	bool IsActive(void) { return mbActive; }

private:
	UIBase* mpUIelems[UIT_MAX];
	bool mbActive;
};
#define gUIMgr UIManager::GetInstance()

#endif