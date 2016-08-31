#ifndef _BattleScene_H_
#define _BattleScene_H_

#include "../../Engine/Scene/Scene.h"

class BattleScene: public Scene
{
public:
	BattleScene(SceneID id);
	virtual ~BattleScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);
	
private:
};

#endif