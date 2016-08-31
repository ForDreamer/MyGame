#ifndef _Fader_H_
#define _Fader_H_

#include "../Hud/Sprite.h"

enum TransitionState
{
	Transition_None = 0,
	Transition_In,
	Transition_Out
};

class Fader: public Sprite
{
public:
	Fader(void);
	~Fader(void);

	void Update(void);
	void Render(void);

	void TransitionIn(void);
	void TransitionOut(void);

	TransitionState GetTransitionState(void) { return mTransState; }
	void SetFadeSpeed(float speed) { mFadeSpeed = speed; }

protected:
	TransitionState mTransState;
	float mFadeSpeed;
	float mAlpha;
};

#endif