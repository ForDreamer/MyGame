#ifndef _MapNavigationScene_H_
#define _MapNavigationScene_H_

#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

class MapNavigationScene: public Scene
{
public:
	MapNavigationScene(SceneID id);
	virtual ~MapNavigationScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);

	static void StartBattle(void);

private:
	void GotoNextScene(void);

private:
	Sprite mSprBlinking;
	float mBlinkTime;

	static bool sbReadyToBattle;
	static float sReadyTime;
};

#endif