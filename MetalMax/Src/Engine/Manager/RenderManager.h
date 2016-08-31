#ifndef _RenderManager_H_
#define _RenderManager_H_

#include <d3dx9.h>
#include "../Misc/Singleton.h"
#include "../Hud/HudObjGroup.h"

class RenderManager: public Singleton<RenderManager>
{
public:
	RenderManager(void);
	~RenderManager(void);

	void Process(void);

	void AddObject(HudObject* pObj) { mHudObjs.AddObj(pObj); }
	void RemoveObject(HudObject* pObj) { mHudObjs.RemoveObj(pObj); }
	void ClearAll(void) { mHudObjs.ClearAll(); }
	int GetSize(void) { return mHudObjs.GetSize(); }

private:
	HudObjGroup mHudObjs;
};
#define gRenderMgr RenderManager::GetInstance()

#endif