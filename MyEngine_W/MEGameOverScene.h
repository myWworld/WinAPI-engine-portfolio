#pragma once
#include "../MyEngine_Source/CommonInclude.h"
#include "../MyEngine_Source/MESceneManager.h"
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MECamera.h"


namespace ME
{


	class GameOverScene: public Scene
	{
	public:
		GameOverScene();
		~GameOverScene();

		void Initialize()override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC mHdc)override;

		void OnEnter() override;
		void OnExit() override;

	private:

		class Camera* mCameraComp;

	};

}
