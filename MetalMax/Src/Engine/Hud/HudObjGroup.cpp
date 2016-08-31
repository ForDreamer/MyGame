#include "HudObjGroup.h"

HudObjGroup::HudObjGroup(void)
{
	mVecObjs.clear();
}

HudObjGroup::~HudObjGroup(void)
{
	mVecObjs.clear();
}

void HudObjGroup::SetVisible(bool visible)
{
	mVisible = visible;
	for(unsigned int i=0; i<mVecObjs.size(); i++)
	{
		mVecObjs[i]->SetVisible(visible);
	}
}

void HudObjGroup::Update(void)
{
	for(unsigned int i=0; i<mVecObjs.size(); i++)
	{
		mVecObjs[i]->Update();
	}
}

void HudObjGroup::Render(void)
{
	for(unsigned int i=0; i<mVecObjs.size(); i++)
	{
		mVecObjs[i]->Render();
	}
}
