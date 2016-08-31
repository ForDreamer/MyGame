#ifndef _Text_H_
#define _Text_H_

#include "HudObject.h"
#include "../../Main/Global.h"

struct TextState
{
	TextState(void)
	{
		x = 0.0f;
		y = 0.0f;
		color = 0xffffffff;
		fontType = FT_SMALLFONT;
		visible = true;
	}

	float x;
	float y;
	DWORD color;
	FontType fontType;
	bool visible;
};

// To Do: add alignment function
class Text: public HudObject
{
public:
	Text(void);
	Text(const char* name);
	virtual ~Text(void);

	float GetPositionX(void) { return mState.x; }
	float GetPositionY(void) { return mState.y; }
	bool GetVisible(void) { return mState.visible; }

	void SetPositionX(float x) { mState.x = x; }
	void SetPositionY(float y) { mState.y = y; }
	void SetPosition(float x, float y) { SetPositionX(x), SetPositionY(y); }
	void SetColor(DWORD color) { mState.color = color; }
	void SetFontType(FontType type) { mState.fontType = type; }
	void SetVisible(bool visible) { mState.visible = visible; }

	void SetText(const char* text, bool createBuffer = false) 
	{
		if(!createBuffer)
		{
			ReleaseTextBuffer();
			mpText = (char*)text;
		}
		else
		{
			// To Do: every time release buffer?
			ReleaseTextBuffer();
			int size = (int)strlen(text) + 1;
			mpText = new char[size];
			strcpy_s(mpText, size, text);
			mbBuffered = true;
		}
	}
	void ReleaseTextBuffer()
	{
		if(mbBuffered)
		{
			SAFE_DELETE(mpText);
			mbBuffered = false;
		}
	}

private:
	TextState mState;
	char* mpText;
	bool mbBuffered;

public:
	void Update(void);
	void Render(void);
};

#endif