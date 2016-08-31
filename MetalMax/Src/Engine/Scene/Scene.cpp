#include "Scene.h"
#include <assert.h>
#include <strsafe.h>
#include "../../Main/Global.h"

#include "../../Game/Scenes/StartupScene.h"
#include "../../Game/Scenes/NameSettingScene.h"
#include "../../Game/Scenes/MapNavigationScene.h"
#include "../../Game/Scenes/InnScene.h"
#include "../../Game/Scenes/WanterScene.h"
#include "../../Game/Scenes/EndingScene.h"
#include "../../Game/Scenes/BattleScene.h"

Scene::Scene(SceneID id)
:	mSceneId(id)
{
}

Scene::~Scene(void)
{
}


DECLARE_SINGLETON(SceneManager);
static SceneManager sSceneManager;

Fader* SceneManager::spFader = NULL;
SceneManager::SceneManager(void)
:	mpSceneActive(NULL)
,	mNextSceneId(SCENE_INVALID)
{
}

SceneManager::~SceneManager(void)
{
	if(spFader)
	{
		delete spFader;
		spFader = NULL;
	}
}

void SceneManager::SetFristScene(SceneID id)
{
	if(!spFader)
		spFader = new Fader();

	assert(!mpSceneActive);
	mNextSceneId = id;
	ActivateNextScene();
}

void SceneManager::SetNextScene(SceneID id)
{
	mNextSceneId = id;
	spFader->TransitionIn();
}

void SceneManager::ActivateNextScene(void)
{
	if(mpSceneActive)
	{
		mpSceneActive->Release();
		delete mpSceneActive;
		mpSceneActive = 0;
	}

	// To Do: use factory mode
	switch(mNextSceneId)
	{
		case SCENE_STARTUP: mpSceneActive = new StartupScene(mNextSceneId); break;
		case SCENE_NAMESETTING: mpSceneActive = new NameSettingScene(mNextSceneId); break;
		case SCENE_MAPNAVIGATION: mpSceneActive = new MapNavigationScene(mNextSceneId); break;
		case SCENE_INN: mpSceneActive = new InnScene(mNextSceneId); break;
		case SCENE_WANTER: mpSceneActive = new WanterScene(mNextSceneId); break;
		case SCENE_ENDING: mpSceneActive = new EndingScene(mNextSceneId); break;
		case SCENE_BATTLE: mpSceneActive = new BattleScene(mNextSceneId); break;
		default: assert(0);
	}
	mNextSceneId = SCENE_INVALID;

	bool result = mpSceneActive->Load();
	assert(result);

	Global::ResetTimer();
	spFader->TransitionOut();
}

void SceneManager::ProcessScene(void)
{
	if(mpSceneActive)
		mpSceneActive->Update();
}
