#include "Fader.h"
#include "Scene.h"
#include "../../Main/Global.h"

Fader::Fader(void)
:	mTransState(Transition_None)
,	mFadeSpeed(4.0f)
,	mAlpha(1.0f)
{
	SetAlignMode(Sprite::ALIGNMODE_CENTER);
	SetBlendMode(Sprite::BLENDMODE_ONE_MINUS_SRCALPHA);
	SetSize((float)Global::gWinWidth+2.0f, (float)Global::gWinHeight+2.0f);
	SetColor(0.0f, 0.0f, 0.0f, 0.0f);
	SetPositionZ(-10.0f);
}

Fader::~Fader(void)
{
}

void Fader::TransitionIn(void)
{
	mTransState = Transition_In;
}

void Fader::TransitionOut(void)
{
	mTransState = Transition_Out;
}

void Fader::Update(void)
{
	if(mTransState != Transition_None)
	{
		if(mTransState == Transition_In)
		{
			mAlpha += mFadeSpeed * Global::GetTimeDelta();
			if(mAlpha >= 1.0f)
			{
				mAlpha = 1.0f;
				gSceneMgr.ActivateNextScene();
			}
		}
		else if(mTransState == Transition_Out)
		{
			mAlpha -= mFadeSpeed * Global::GetTimeDelta();
			if(mAlpha <= 0.0f)
			{
				mAlpha = 0.0f;
				mTransState = Transition_None;
			}
		}
		Global::ClearKeyState();

		SetAlpha(mAlpha);
		Sprite::Update();
	}
}

void Fader::Render(void)
{
	if(mTransState != Transition_None)
	{
		Sprite::Render();
	}
}