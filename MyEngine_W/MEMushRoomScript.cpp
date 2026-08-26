#include "MEMushRoomScript.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METime.h"
#include "MEMonsters.h"
#include "MEBullet.h"
#include "MEPlayerScript.h"

namespace ME
{
	MushRoomScript::MushRoomScript()
		:mSpeed(25.0f)
		, mHp(100.0f)
		, mRespawnTime(0.0f)
		, mbIsDead(false)
		, mbIsRespawn(false)
		, mbIsHurtState(false)
	{
	}
	MushRoomScript::~MushRoomScript()
	{
	}
	void MushRoomScript::Initialize()
	{
	
		
	}
	void MushRoomScript::Update()
	{

		if (mAnimator == nullptr)
		{
			mAnimator = GetOwner()->GetComponent<Animator>();
		}

		if (mHp == 0)
		{
			mState = eState::Die;
		}

		if (mbIsHurtState == true)
		{
			mState = eState::Hurt;
		}

		switch (mState)
		{
		case ME::MushRoomScript::eState::Idle:
			Idle();
			break;
		case ME::MushRoomScript::eState::Walk:
			Move();
			break;
		case ME::MushRoomScript::eState::Die:
			Die();
			break;
		case ME::MushRoomScript::eState::Hurt:
			Hurt();
			break;
		default:
			break;
		}
	}

	void MushRoomScript::Idle()
	{
		mTime += Time::DeltaTime();

		if (mTime > 1.0f)
		{
			mState = eState::Walk;
			int direction = rand() % 3;
			mDirection = (eDirection)direction;
			PlayAnimationByDirection(mDirection);

			mTime = 0;
		}
	}

	void MushRoomScript::Move()
	{
		mTime += Time::DeltaTime();


		if (mDirection == eDirection::End || mTime > 2.0f)
		{
			mState = eState::Idle;
			mAnimator->PlayAnimation(L"IdleL", false);
		}

		Transform* tr = GetOwner()->GetComponent<Transform>();

		Translate(tr);

	}

	void MushRoomScript::Respawn()
	{
		if (mbIsRespawn == true)
		{
			GetOwner()->SetNoRender(false);
			
		}
	}

	void MushRoomScript::Die()
	{
		GameObject::eState state = GetOwner()->GetState();

		if (state == GameObject::eState::Active && mbIsDead == false)
		{
			mbIsDead = true;
			mbIsRespawn = true;

			static_cast<Monsters*>(GetOwner())->SetMonsterState(Monsters::eState::Dead);

			mAnimator->PlayAnimation(L"DeadR", false);
			PlayerScript::PlusScore();

		}

		if (state == GameObject::eState::NoRender)
		{

			mRespawnTime += Time::DeltaTime();

			if (mRespawnTime > 8.0f)
			{

				mRespawnTime = 0.0f;
				mState = eState::Idle;

				GetOwner()->SetNoRender(true);
				static_cast<Monsters*>(GetOwner())->SetMonsterState(Monsters::eState::Alive);
				mAnimator->PlayAnimation(L"idleL", true);


				mHp = 100.0f;
				mbIsDead = false;
				mbIsRespawn = false;
			}

		}

	}
	
	void MushRoomScript::Hurt()
	{
		if (mAnimator->IsComplete())
		{
			if (mbIsHurtState == true)
			{
				Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();
				playerRb->StopMoving();

				mbIsHurtState = false;
				mState = eState::Idle;
				mAnimator->PlayAnimation(L"idleL");
			}
		}

	}

	void MushRoomScript::Translate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();

		switch (mDirection)
		{
		case ME::MushRoomScript::eDirection::Left:
			pos += Vector2::Left * (mSpeed * Time::DeltaTime());
			break;
		case ME::MushRoomScript::eDirection::Right:
			pos += Vector2::Right * (mSpeed * Time::DeltaTime());
			break;
		case ME::MushRoomScript::eDirection::End:
			break;
		default:
			break;
		}

		tr->SetPosition(pos);
	}

	void MushRoomScript::PlayAnimationByDirection(eDirection direction)
	{
		switch (direction)
		{
		case ME::MushRoomScript::eDirection::Left:
			mAnimator->PlayAnimation(L"MushRoomLeftWalkR", true);
			break;
		case ME::MushRoomScript::eDirection::Right:
			mAnimator->PlayAnimation(L"MushRoomRightWalkL" ,true);
			break;
		case ME::MushRoomScript::eDirection::End:
			mAnimator->PlayAnimation(L"idleL", false);
			break;
		default:
			break;
		}
	}

	void MushRoomScript::LateUpdate()
	{
	}
	void MushRoomScript::Render(HDC hdc)
	{
	}
	void MushRoomScript::OnCollisionEnter(Collider* other)
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

				if(mbIsHurtState == false)
					GotHitByBullet(bullet);

				mHp -= 10;
			}
		}

		if (other->GetName() == L"Player")
		{
			return;
		}
	}

	void MushRoomScript::OnCollisionStay(Collider* other)
	{
	}

	void MushRoomScript::OnCollisionExit(Collider* other)
	{
	}

	float MushRoomScript::DetermineLeftOrRightByVector(GameObject* obj)
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

	void MushRoomScript::GotHitByBullet(GameObject* bullet)
	{
		Rigidbody* mushroomRb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 mushroomVelocity = mushroomRb->GetVelocity();

		float leftOrRight = DetermineLeftOrRightByVector(bullet);

		if (leftOrRight >= 0)
		{
			mushroomVelocity.x += 50.0f;
		}
		else
		{
			mushroomVelocity.x -= 50.0f;

		}
		mushroomVelocity.y -= 100.0f;

		mushroomRb->SetVelocity(mushroomVelocity);
		mushroomRb->SetGround(false);

		mbIsHurtState = true;
		PlayHurtAnimation();
	}

	void MushRoomScript::PlayHurtAnimation()
	{
		GameObject* monster = GetOwner();
		MushRoomScript* script = monster->GetComponent<MushRoomScript>();
		
		if(mAnimator == nullptr )
			mAnimator = monster->GetComponent<Animator>();

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

}