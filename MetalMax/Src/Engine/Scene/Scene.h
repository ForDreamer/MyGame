#ifndef _Scene_H_
#define _Scene_H_

#include "../Misc/Singleton.h"
#include "../Misc/PreDefine.h"
#include "Fader.h"

enum SceneID
{
	SCENE_INVALID = -1,
	SCENE_STARTUP = 0,	/*初始场景*/
	SCENE_NAMESETTING = 1,/*设置名字场景*/
	SCENE_MAPNAVIGATION = 2, /*世界场景*/
	SCENE_INN = 3,
	SCENE_WANTER = 4,	/*通缉图场景*/
	SCENE_ENDING = 5,	/*结束场景*/
	SCENE_BATTLE = 6
};

class Scene
{
public:
	Scene(SceneID id);
	virtual ~Scene(void);

	virtual bool Load(void) = 0;
	virtual void Release(void) = 0;
	virtual void Update(void) = 0;

protected:
	SceneID mSceneId;
};

#define MAX_SCENES 32
class SceneManager: public Singleton<SceneManager>
{
public:
	SceneManager(void);
	~SceneManager(void);

	void SetFristScene(SceneID id);
	void SetNextScene(SceneID id); // activate fader transition in, then goto next scene
	void ActivateNextScene(void); // goto next scene immediately
	void ProcessScene(void);

	static Fader* GetFader(void) { return spFader; }
	bool IsFading(void) { return (spFader->GetTransitionState() != Transition_None); }

private:
	Scene* mpSceneActive;
	SceneID mNextSceneId;

private:
	static Fader* spFader;
};
#define gSceneMgr SceneManager::GetInstance()

#endif