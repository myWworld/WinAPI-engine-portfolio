#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "../MyEngine_Source/MEAnimator.h"
#include "METransform.h"
#include "../MyEngine_Source/MECollider.h"

namespace ME
{


	class TurtleScript :public Script
	{
	public:

		enum class eState
		{
			Idle,
			Walk,
			Die,
			Hurt,
			Shell,

		};

		enum class eDirection
		{
			Left,
			Right,
			End,
		};

		TurtleScript();
		~TurtleScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		eDirection GetDirection() { return mDirection; }

		void Respawn();

		float GetHp() { return mHp; }

		void Hurt();

		bool IsTouchedShell() { return mbIsTouchShell; }
		void SetTouchedShell(bool isTouchedShell) { mbIsTouchShell = isTouchedShell; }
		
		void SetIsShell(bool isShell) { mbIsShell = isShell; }
		bool IsShell() { return mbIsShell; }


		void SetHead(GameObject* head) { mHead = head; }
		void MakeHeadNull() { mHead = nullptr; }

		void MakeTurtleIntoShell();
		void AdjustColForHeadAndBody();

	private:

		void Idle();
		void Move();
		void Die();
		void Shell();

		void PlayAnimationByDirection(eDirection direction);
		void Translate(Transform* tr);
		void HeadTranslate(Transform* tr);
		
		

		float DetermineLeftOrRightByVector(GameObject* obj);
		void GotHitByBullet(GameObject* bullet);
		void PlayHurtAnimation();
		bool IsHeadShot();

		void GetCenterPos(GameObject* player);
		void MakePlayerBounce(Collider* player);

		void AdjustHeadAfterHurt();


	private:

		eState mState;
		eDirection mDirection;

		float mTime;
		float mSpeed;
		float mShellSpeed;

		class Animator* mAnimator;

		float mRespawnTime;
		float mHp;

		bool mbIsDead;
		bool mbIsRespawn;
		bool mbIsHurtState;

		bool mbIsHeadShot;
		bool mbIsShell;
		bool mbIsTouchShell;

		Vector2 mMonsterCenterPos;
		Vector2 mPlayerCenterPos;

		GameObject* mHead;
	};

}
