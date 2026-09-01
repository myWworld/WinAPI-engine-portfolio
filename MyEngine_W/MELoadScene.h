#pragma once
#include "../MyEngine_Source/CommonInclude.h"
#include "../MyEngine_Source/MESceneManager.h"
#include "MEStage1.h"
#include "MEEndScene.h"
#include "MEScoreScene.h"
#include "METitleScene.h"
#include "MEGameOverScene.h"
#include "METoolScene.h"
#include "MEStage2.h"
#include "MEStage3.h"

namespace ME
{
	void LoadScenes()
	{
		
		SceneManager::CreateScene<ToolScene>(L"ToolScene");
 		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		SceneManager::CreateScene<Stage1>(L"Stage1");
		SceneManager::CreateScene<Stage2>(L"Stage2");
		SceneManager::CreateScene<Stage3>(L"Stage3");
		SceneManager::CreateScene<GameOverScene>(L"GameOverScene");

		SceneManager::LoadScene(L"Stage3");
		//SceneManager::LoadScene(L"TitleScene");
	

	}
}
