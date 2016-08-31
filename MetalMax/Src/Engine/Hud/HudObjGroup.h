#ifndef _HudObjGroup_H_
#define _HudObjGroup_H_

#include "HudObject.h"
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

class HudObjGroup
{
	typedef vector<HudObject*> HudObjList;
	typedef HudObjList::iterator ListIter;
public:
	HudObjGroup(void);
	virtual ~HudObjGroup(void);

public:
	virtual bool GetVisible(void) { return mVisible; }
	virtual void SetVisible(bool visible);
	virtual void SetPosition(int id, float x, float y) { mVecObjs[id]->SetPosition(x, y); }

	int GetSize(void) { return (int)mVecObjs.size(); }
	void AddObj(HudObject* pObj) 
	{
		assert(pObj);
		assert(GetObj(pObj) == NULL);

		const char* itemName = pObj->GetName();
		mVecObjs.push_back(pObj);
	}

	void RemoveObj(HudObject* pObj)
	{
		assert(pObj);
		ListIter iter = find(mVecObjs.begin(), mVecObjs.end(), pObj);
		if (iter != mVecObjs.end())
			mVecObjs.erase(iter);
	}

	void ClearAll(void)
	{
		mVecObjs.clear();
	}

	HudObject* GetObj(HudObject* pObj)
	{
		ListIter iter = find(mVecObjs.begin(), mVecObjs.end(), pObj);
		if (iter != mVecObjs.end())
			return (*iter);
		return NULL;
	}

	HudObject* GetObj(const char* name) 
	{
		for(unsigned int i=0; i<mVecObjs.size(); i++)
		{
			if(mVecObjs[i]->GetName() == name)
				return mVecObjs[i];
		}
		return NULL;
	}

protected:
	HudObjList mVecObjs;
	bool mVisible;

private:
	friend class RenderManager;
	virtual void Update(void);
	virtual void Render(void);
};

#endif