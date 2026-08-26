#pragma once
#include "../MyEngine_Source/METexture.h"
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MEAnimator.h"
#include "../MyEngine_Source/MECamera.h"

namespace ME
{

	class Stage1 : public Scene
	{
	public:
	
		Stage1(); 
		~Stage1();
		

	 void Initialize() override;
	 void Update() override;
	 void LateUpdate() override;
	 void Render(HDC mHdc) override;

	 void OnEnter() override;
	 void OnExit() override;

	private:

		void CreatePlayerAnimation(Animator *animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		void CreateMushRoomAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		void CreateSkeletonAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		void CreateTurtleAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture);
		
		void CreateMushRoom(GameObject* mushroom);
		void CreateSkeleton(GameObject* skeleton);
		void CreateCannon();
		void CreateTurtle(GameObject* turtle);

		void CreateQbox(GameObject* qbox);
		void CreateFloor(GameObject* floor, float xSize= 1.0f, float ySize = 1.0f, float xOffset= 0.0f, float yOffset=0.0f);
		void CreateChimney(GameObject* chimney, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateStair(Vector2 startPos, int maxSteps, int height, char direction);
		void CreateFlag(GameObject* flag, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateBlock(GameObject* block, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		void CreateLastDoor(GameObject* lastdoor, float xSize = 1.0f, float ySize = 1.0f, float xOffset = 0.0f, float yOffset = 0.0f);

		void playerInitialize();

	private:

		class GameObject* mPlayer;
		class GameObject* mEffect;
		class Camera* mCameraComp;

		float mPlayTime;
	};
}
