#ifndef _StartupScene_H_
#define _StartupScene_H_

#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

class StartupScene: public Scene
{
	enum TextureIndex
	{
		TEX_STARTUP_BG = 0,
	};

public:
	StartupScene(SceneID id);
	virtual ~StartupScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);
	
private:
	void ResetIconPos(void);
	void StartGame(void);

private:
	Sprite mSprLogo;
	Text mTxtInfo;
	bool mbHasSaveFile;

	Text mTxtNewGame;
	Text mTxtContinue;
	Text mTxtExit;
	Sprite mIcon;
	int mSelectIndex;
};

#endif