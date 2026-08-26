#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "../MyEngine_Source/MEAnimator.h"
#include "../MyEngine_Source/MECollider.h"
#include "../MyEngine_Source/METexture.h"
#include "../MyEngine_Source/MERigidbody.h"

namespace ME
{


	class PlayerScript :public Script
	{
	public:
		enum class eState
		{	
			Standing,
			StandAttack,
			RunningAttack,
			Jump,
			Run,
			Walk,
			Fall,
			GetDown,
			Move,
			Hurt,
			Die,
			Clear,
			End,
		};

		enum class ePrevDirection
		{
			Left,
			Right
			
		};

		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;
		
		void MakeBullet(bool isRunning = false);

		void PlayStandingAnimByPrevDirection();


		ePrevDirection GetPrevDirection() { return mPrevDirection; }

		static void PlusScore() { PlayerScript::mScore += 10.0f; }
		static float GetHp() { return mHp; }
		static void ReSetHp(float hp) { mHp = hp; }
		static void ReSetScore(int score) { mScore = score; }

		static bool IsStar() { return mbIsStar; }
		static bool IsHurtState() { return mbIsHurtState; }

		void SetEffect(GameObject* effect) { mEffect = effect; }

		void SetPixelTexture(graphics::Texture* texture) { mPixelTexture = texture; }

	private:

		void Standing();
		void Move();
		void DetermineMovingVelocity();

		void Jump();
		void Run();
		void RunningAttack();
		void StandingAttack();
		void Die();
		void Hurt();
		void StageClear();

		void PrintScore(HDC hdc);
		void RunAttackTime();
		
	
		void PlayBulletByPrveDirection(Animator* animator);
		void PlayJumpAnimationByPrevDirection();
		void PlayHurtAnimationByMonster(float rightOrLeft);
		void PlayAuraAnimation();

		void IsDie();
		void IsStarMode();
		void IsAxeBroken();
		float DetermineLeftOrRightByVector(GameObject* obj);

		void GetHurtAccordingToHp(GameObject* obj);


		GameObject* CreateAura();



	private:

		bool isJump;
		float jumpSeconds;

		float mAxeTime;
		bool mbIsAxeGone;

		float mAttackTime;
		bool mbIsRunningAttack;
		
		static bool mbIsStar;
		float mStarTime;
		bool mbStillStartTime;

		static float mHp;
		static int mScore;
		static bool mbIsHurtState;

		float mHurtCount;
		bool mbHurtCountStart;

		bool mbIsOnFlag;
	
		eState mState;
		ePrevDirection mPrevDirection;

		class Animator* mAnimator;
		class GameObject* mEffect;

		graphics::Texture* mPixelTexture;


	};
}

