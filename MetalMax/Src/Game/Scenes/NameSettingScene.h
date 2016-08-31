#ifndef _NameSettingScene_H_
#define _NameSettingScene_H_

#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Hud/Sprite.h"
#include "../../Engine/Hud/Text.h"

#define NS_KEY_NUM 104
#define NS_KEY_BACK 1000
#define NS_KEY_END 2000
#define NS_KEY_EIDT_MAX 4

enum UserType
{
	UT_PLAYER = 0,
	UT_TANK = 1
};

class NameSettingScene: public Scene
{
	enum TextureIndex
	{
		TEX_NAMESETTING_BG = 0,
		TEX_ICON_TOP = 1,
		TEX_ICON_BOT = 2,
	};

public:
	struct KeyInfo
	{
		char key[4];
	};
	static UserType sUserType;

public:
	NameSettingScene(SceneID id);
	virtual ~NameSettingScene(void);

	virtual bool Load(void);
	virtual void Release(void);
	virtual void Update(void);
	
private:
	void InitKeyBoard(void);
	void InitTopText(void);
	void UpdateTopText(void);
	void GotoNextScene(void);
	void ResetSprBotPos(void);
	void ResetSprTopPos(void);

private:
	Sprite mSprBG;
	Text mTxtInfo;
	Text mTxtBack;
	Text mTxtEnd;

	Sprite mIconTop;
	Sprite mIconBot;
	Text mKeys[NS_KEY_NUM];

	int mCurKeyIndex;
	int mCurEditIndex;

	float mfCurXAccum;
	float mfCurYAccum;

	char mName[NS_KEY_EIDT_MAX];
	Text mTxtName[NS_KEY_EIDT_MAX];
};

#endif