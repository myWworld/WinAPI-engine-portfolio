#pragma once
#include "../MyEngine_Source/METexture.h"
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MEAnimator.h"
#include "../MyEngine_Source/MECamera.h"

namespace ME
{

	class Stage2 : public Scene
	{
	public:

		Stage2();
		~Stage2();


		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC mHdc) override;

		void OnEnter() override;
		void OnExit() override;

	private:

		void CreatePlayerAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		void CreateMushRoomAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		void CreateSkeletonAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);


		void CreateMushRoom(GameObject* mushroom);
		void CreateSkeleton(GameObject* skeleton);
		void CreateCannon();

		void CreateQbox(GameObject* qbox);
		void CreateFloor(GameObject* floor, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateChimney(GameObject* chimney, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateStair(Vector2 startPos, int maxSteps, int height, char direction);
		void CreateFlag(GameObject* flag, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateBlock(GameObject* block, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateMovingBlock(GameObject* block, char direction
			, float maxRangeVal, float minRangeVal
			, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		
		void CreatePulleyBlock(GameObject* left, GameObject* right
			, float xLeftSize = 1.0f, float yLeftSize = 1.0f, float xRightSize = 1.0f, float yRightSize = 1.0f
			, float xLeftOffset = 0.0f, float yLeftOffset = 0.0f, float xRightOffset = 0.0f, float yRightOffset = 0.0f);
		
		void CreateOnlyDownBlock(GameObject* block, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		
		void CreateTrampoline(GameObject* trampoline
			, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		
		
		
		void CreateLastDoor(GameObject* lastdoor, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);

		void playerInitialize();

		void MovingBlocks();
		void Floors();

	private:

		class GameObject* mPlayer;
		class GameObject* mEffect;
		class Camera* mCameraComp;

		float mPlayTime;

		float mCannonLaunchTime;
	};
}

