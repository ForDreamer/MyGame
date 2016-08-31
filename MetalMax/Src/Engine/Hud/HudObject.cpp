#include "HudObject.h"

HudObject::HudObject(void)
{
	mpName = "Default_HudObject_Name";
}

HudObject::HudObject(const char* name)
{
	mpName = name;
}

HudObject::~HudObject(void)
{
}