#ifndef _WanterScene_H_
#define _WanterScene_H_

#include "../../Engine/Scene/Scene.h"

class WanterScene: public Scene
{
	enum TextureIndex
	{
		TEX_WANTER_BG = 0,
	};

public:
	WanterScene(SceneID id);
	virtual ~WanterScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);
	
private:
	Sprite mSprBG;
};

#endif