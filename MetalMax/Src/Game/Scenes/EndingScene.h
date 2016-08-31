#ifndef _EndingScene_H_
#define _EndingScene_H_

#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Hud/Text.h"

class EndingScene: public Scene
{
public:
	EndingScene(SceneID id);
	virtual ~EndingScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);
	
private:
	Text mTxtEnd;
	Text mTxtPresent;
	Text mTxtQQ;
	Text mTxtEmail;
	static int sEngStep;
};

#endif