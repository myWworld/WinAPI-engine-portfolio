#pragma once
#include "../MyEngine_Source/METexture.h"
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MEAnimator.h"
#include "../MyEngine_Source/MECamera.h"

namespace ME
{

	class Stage3 : public Scene
	{
	public:

		Stage3();
		~Stage3();


		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC mHdc) override;

		void OnEnter() override;
		void OnExit() override;

	private:

		void CreatePlayerAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		void CreateKoopa(GameObject* koopa);

		void CreateQbox(GameObject* qbox);
		void CreateFloor(GameObject* floor, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateCeiling(GameObject* ceiling, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		
		void CreateStair(Vector2 startPos, int maxSteps, int height, char direction);
		void CreateFlag(GameObject* flag, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateBlock(GameObject* block, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
	
		void CreateLastDoor(GameObject* lastdoor, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);

		void CreateFireBar(GameObject* firebar, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f, float startTime = 1.0f);

		void CreateLavaBubble(GameObject* lavabubble, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f, float startTime = 1.0f, float lauchTime = 160.0f);

		void CreateBridgeBlocks();

		void MakeFloor();
		void MakeCeiling();
		void MakeFireBar();
		void MakeLavaBubble();


		void playerInitialize();

	private:

		class GameObject* mPlayer;
		class GameObject* mEffect;
		class Camera* mCameraComp;

		float mPlayTime;

		float mCannonLaunchTime;
	};
}

