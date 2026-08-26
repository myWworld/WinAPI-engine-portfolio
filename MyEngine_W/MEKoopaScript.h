#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "../MyEngine_Source/MEAnimator.h"
#include "METransform.h"
#include "../MyEngine_Source/MECollider.h"

namespace ME
{


	class KoopaScript :public Script
	{
	public:

		enum class eState
		{
			Idle,
			Walk,
			Jump,
			Fire,
			Die,
			Hurt,

		};

		enum class eDirection
		{
			Left,
			Right,
			End,
		};

		KoopaScript();
		~KoopaScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetEnemey(GameObject* player) { mPlayer = player; }

		eDirection GetDirection() { return mDirection; }

		float GetHp() { return mHp; }

		void Hurt();


	private:

		void Idle();
		void Move();
		void Die();
		void Jump();
		void FireLaunch();

		void MakeFire();
		Vector2 GetMouthPos();

		void PlayAnimationByDirection(eDirection direction);
		void Translate(Transform* tr);

		float DetermineLeftOrRightByVector(GameObject* obj);
		void GotHitByBullet(GameObject* bullet);
		void PlayHurtAnimation();

		void FollowingCharacterDirection();

	private:

		eState mState;
		eDirection mDirection;

		float mTime;
		float mFireTime;
		
		GameObject* mPlayer;

		class Animator* mAnimator;

		float mHp;

		Vector2 mMouthPos;

		bool mbIsDead;
		bool mbIsHurtState;
	};

}

