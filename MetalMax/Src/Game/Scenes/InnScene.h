#ifndef _InnScene_H_
#define _InnScene_H_

#include "../../Engine/Scene/Scene.h"

class InnScene: public Scene
{
public:
	InnScene(SceneID id);
	virtual ~InnScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);
	
private:
	float mTimer;
};

#endif