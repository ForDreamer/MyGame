#ifndef _UIBase_H_
#define _UIBase_H_

class UIBase
{
public:
	UIBase(void);
	virtual ~UIBase(void);
	
	virtual void Init(void) = 0;
	virtual void Update(float baseX, float baseY) = 0;
	virtual void Render(void) = 0;

	virtual void OpenUI(void) { SetVisible(true); SetOnFocus(true); }
	virtual void CloseUI(void)  { SetVisible(false); SetOnFocus(false); }

	virtual void SetVisible(bool visible) { mbVisible = visible; }
	virtual void SetOnFocus(bool focus) 
	{ 
		mbOnFocus = focus;
		if(mbOnFocus)
			ShowSelector();
		else
			HideSelector();	
	}
	virtual bool IsVisible(void) { return mbVisible; }
	virtual bool IsOnFocus(void) { return mbOnFocus; }

	virtual void ShowSelector(void) {}
	virtual void HideSelector(void) {}

protected:
	bool mbVisible;
	bool mbOnFocus;

	int mSelectId;
};

#endif