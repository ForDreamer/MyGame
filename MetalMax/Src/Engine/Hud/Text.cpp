#include "Text.h"

Text::Text(void)
:	HudObject()
,	mpText(NULL)
,	mbBuffered(false)
{
}

Text::Text(const char* name)
:	HudObject(name)
,	mpText(NULL)
,	mbBuffered(false)
{
}

Text::~Text(void)
{
	ReleaseTextBuffer();
}

void Text::Update(void)
{
}

void Text::Render(void)
{
	if(mState.visible)
	{
		Global::PrintText(mState.x, mState.y, mpText, mState.fontType, mState.color);
	}
}
