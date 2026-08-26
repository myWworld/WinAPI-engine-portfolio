#include "MESkeletonScript.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METime.h"
#include "MEBullet.h"
#include "MEBulletScript.h"
#include "MEPlayerScript.h"
#include "MEMonsters.h"

namespace ME
{
	SkeletonScript::SkeletonScript()
		:mTime(0.0f)
		, mHp(100.0f)
		, mRespawnTime(0.0f)
		,mbIsDead(false)
		,mbISRespawn(false)
	{
	}
	SkeletonScript::~SkeletonScript()
	{
	}
	void SkeletonScript::Initialize()
	{
	}
	void SkeletonScript::Update()
	{
		if (mAnimator == nullptr)
		{
			mAnimator = GetOwner()->GetComponent<Animator>();
		}

		if (mHp == 0)
		{				
			mSkeletonState = eState::Die;

		}


		switch (mSkeletonState)
		{
		case ME::SkeletonScript::eState::Idle:
			Idle();
			break;
		case ME::SkeletonScript::eState::Shout:
			Shout();
			break;
		case ME::SkeletonScript::eState::Attack:
			Attack();
			break;
		case ME::SkeletonScript::eState::Walk:
			Move();
			break;
		case ME::SkeletonScript::eState::Hurt:
			Hurt();
			break;
		case ME::SkeletonScript::eState::Die:
			Die();
			break;
		default:
			break;
		}
	}

	void SkeletonScript::Idle()
	{
		mTime += Time::DeltaTime();

		if (mTime > 1.0f)
		{
			int state = rand() % 4;
			int direction = rand() % 2;

			mDirection = (eDirection)direction;
			mSkeletonState = (eState)state;

			PlayAnimationByStateAndDirection();

			mTime = 0;
		}
	}


	void SkeletonScript::Shout()
	{
		if (mAnimator->IsComplete())
		{
			mSkeletonState = eState::Idle;
		}
	}

	void SkeletonScript::Attack()
	{
		if (mAnimator->IsComplete())
		{
			mSkeletonState = eState::Idle;
		}
	}
	
	void SkeletonScript::Move()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		mTime += Time::DeltaTime();

	
		Translate(tr);

		if (mTime > 2.5f)
		{
			mSkeletonState = eState::Idle;
			mTime = 0;
		}
	}

	void SkeletonScript::Die()
	{
		if (GetOwner()->GetState() == GameObject::eState::Active && mbIsDead == false)
		{
			mbIsDead = true;
			mbISRespawn = true;

			static_cast<Monsters*>(GetOwner())->SetMonsterState(Monsters::eState::Dead);

			if (mDirection == SkeletonScript::eDirection::Left)
			{
				mAnimator->PlayAnimation(L"DeadL", false);
			}
			else if (mDirection == SkeletonScript::eDirection::Right)
			{
				mAnimator->PlayAnimation(L"DeadR", false);
			}
			PlayerScript::PlusScore();
		}

		if (GetOwner()->GetState() == GameObject::eState::NoRender)
		{

			mRespawnTime += Time::DeltaTime();

			if (mRespawnTime > 8.0f)
			{

				mRespawnTime = 0.0f;
				mSkeletonState = eState::Idle;
				
				GetOwner()->SetNoRender(true);

				static_cast<Monsters*>(GetOwner())->SetMonsterState(Monsters::eState::Alive);
				mHp = 100.0f;
				mbIsDead = false;
				mbISRespawn = false;
			}
			
		}

	
	}

	void SkeletonScript::Hurt()
	{
		if (mAnimator->IsComplete())
		{
			if (mbIsHurtState == true)
			{
				Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();
				playerRb->StopMoving();

				mbIsHurtState = false;
				mSkeletonState = eState::Idle;
				PlayAnimationByStateAndDirection();
			}
		}
	}

	float SkeletonScript::DetermineLeftOrRightByVector(GameObject* obj)
	{
		Transform* objTr = obj->GetComponent<Transform>();

		Transform* playerTr = GetOwner()->GetComponent<Transform>();
		Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();

		Vector2 playerColPos = playerTr->GetPosition() + GetOwner()->GetComponent<Collider>()->GetOffset();
		Vector2 monsterColPos = objTr->GetPosition() + obj->GetComponent<Collider>()->GetOffset();

		Vector2 playerColliderSize = GetOwner()->GetComponent<Collider>()->GetSize() * 100.0f;
		Vector2 monsterColliderSize = obj->GetComponent<Collider>()->GetSize() * 100.0f;

		Vector2 playerCenterColPos = playerColPos + (playerColliderSize / 2.0f);
		Vector2 monsterCenterColPos = monsterColPos + (monsterColliderSize / 2.0f);

		Vector2 leftOrRight = playerCenterColPos - monsterCenterColPos;

		return leftOrRight.x;
	}

	void SkeletonScript::GotHitByBullet(GameObject* bullet)
	{
		Rigidbody* skeletonRb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 skeletonVelocity = skeletonRb->GetVelocity();

		float leftOrRight = DetermineLeftOrRightByVector(bullet);

		if (leftOrRight >= 0)
		{
			skeletonVelocity.x += 50.0f;
		}
		else
		{
			skeletonVelocity.x -= 50.0f;

		}
		skeletonVelocity.y -= 100.0f;

		skeletonRb->SetVelocity(skeletonVelocity);
		skeletonRb->SetGround(false);

		mbIsHurtState = true;
		mSkeletonState = eState::Hurt;

		PlayHurtAnimation();
	}

	void SkeletonScript::PlayHurtAnimation()
	{
		GameObject* monster = GetOwner();
		SkeletonScript* script = monster->GetComponent<SkeletonScript>();

		if (mHp > 0)
		{
			if (mDirection == eDirection::Left)
			{
				mAnimator->PlayAnimation(L"HurtL", false);
			}
			else if (mDirection == eDirection::Right)
			{
				mAnimator->PlayAnimation(L"HurtR", false);
			}
		}
		else
		{
			return;

		}
	}

	void SkeletonScript::Respawn()
	{
		if(mbISRespawn == true)
			GetOwner()->SetNoRender(false);

	}

	void SkeletonScript::Translate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();

		switch (mDirection)
		{
		case ME::SkeletonScript::eDirection::Right:
			pos.x += 20 * Time::DeltaTime();
			break;
		case ME::SkeletonScript::eDirection::Left:
			pos.x -= 20 * Time::DeltaTime();
			break;
		default:
			break;
		}

		tr->SetPosition(pos);

	}
		

	void SkeletonScript::PlayAnimationByStateAndDirection()
	{
		if (mDirection == eDirection::Right)
		{
			switch (mSkeletonState)
			{
			case ME::SkeletonScript::eState::Idle:
				mAnimator->PlayAnimation(L"SkeletonIdleR");
				break;
			case ME::SkeletonScript::eState::Shout:
				mAnimator->PlayAnimation(L"SkeletonShoutR",false);
				break;
			case ME::SkeletonScript::eState::Attack:
				mAnimator->PlayAnimation(L"SkeletonAttackR", false);
				break;
			case ME::SkeletonScript::eState::Walk:
				mAnimator->PlayAnimation(L"SkeletonWalkR");
				break;
			default:
				break;
			}
		}
		else if (mDirection == eDirection::Left)
		{
			switch (mSkeletonState)
			{
			case ME::SkeletonScript::eState::Idle:
				mAnimator->PlayAnimation(L"SkeletonIdleL");
				break;
			case ME::SkeletonScript::eState::Shout:
				mAnimator->PlayAnimation(L"SkeletonShoutL", false);
				break;
			case ME::SkeletonScript::eState::Attack:
				mAnimator->PlayAnimation(L"SkeletonAttackL", false);
				break;
			case ME::SkeletonScript::eState::Walk:
				mAnimator->PlayAnimation(L"SkeletonWalkL");
				break;
			default:
				break;
			}
		}
	}

	void SkeletonScript::LateUpdate()
	{
	}
	void SkeletonScript::Render(HDC hdc)
	{
	}
	void SkeletonScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Bullet")
		{
			Bullet* bullet = static_cast<Bullet*>(other->GetOwner());

			if (mHp == 0)
				return;
			else
			{
				bullet->SetActive(false);
				bullet->SetDeath();
				mHp -= 10;

				if (mbIsHurtState == false)
				{
					GotHitByBullet(bullet);
				}

				int a = 0;

			}
			
		}
	}
	void SkeletonScript::OnCollisionStay(Collider* other)
	{
	}
	void SkeletonScript::OnCollisionExit(Collider* other)
	{
	}
}