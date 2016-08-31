#ifndef _HudObject_H_
#define _HudObject_H_

#include "../Misc/PreDefine.h"

class HudObject
{
public:
	HudObject(void);
	HudObject(const char* name);
	virtual ~HudObject(void);

	virtual float GetPositionX(void) = 0;
	virtual float GetPositionY(void) = 0;
	virtual bool GetVisible(void) = 0;

	virtual void SetPositionX(float x) = 0;
	virtual void SetPositionY(float y) = 0;
	virtual void SetPosition(float x, float y) = 0;
	virtual void SetVisible(bool visible) = 0;

	const char* GetName(void) { return mpName; }

protected:
	const char* mpName;

protected:
	friend class HudObjGroup;
	virtual void Update(void) = 0;
	virtual void Render(void) = 0;
};

#endif