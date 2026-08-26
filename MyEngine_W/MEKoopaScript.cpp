#include "MEKoopaScript.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METime.h"
#include "MEBoxCollider2D.h"
#include "MEObject.h"
#include "MEMonsters.h"
#include "MEBullet.h"
#include "MEPlayerScript.h"
#include "MEResources.h"
#include "MEFire.h"
#include "MEFireScript.h"


namespace ME
{
	KoopaScript::KoopaScript()
		:mTime(0.0f)
		,mState(eState::Idle)
		,mPlayer(nullptr)
	{
	}
	KoopaScript::~KoopaScript()
	{
	}
	void KoopaScript::Initialize()
	{
	}
	void KoopaScript::Update()
	{
		if (mAnimator == nullptr)
			mAnimator = GetOwner()->GetComponent<Animator>();
	
		mFireTime += Time::DeltaTime();

		FollowingCharacterDirection();
		
		switch (mState)
		{
		case ME::KoopaScript::eState::Idle:
			Idle();
			break;
		case ME::KoopaScript::eState::Walk:
			Move();
			break;
		case ME::KoopaScript::eState::Jump:
			Jump();
			break;
		default:
			break;
		}
		

	}
	void KoopaScript::LateUpdate()
	{
	}
	void KoopaScript::Render(HDC hdc)
	{
	}
	void KoopaScript::OnCollisionEnter(Collider* other)
	{
	}
	void KoopaScript::OnCollisionStay(Collider* other)
	{
	}
	void KoopaScript::OnCollisionExit(Collider* other)
	{
	}
	void KoopaScript::Hurt()
	{
	}
	void KoopaScript::Idle()
	{
		mTime += Time::DeltaTime();

		if (mDirection == eDirection::Left)
		{
			mAnimator->PlayAnimation(L"IdleL");
		}
		if (mDirection == eDirection::Right)
		{
			mAnimator->PlayAnimation(L"IdleR");
		}

		if (mTime > 2.0f)
		{
			mTime = 0.0f;

			int state = rand() % 10 + 1;

			if (state >= 1 && state <= 7)
				mState = eState::Jump;
			else
				mState = eState::Walk;
		
			int direction = rand() % 2;
			mDirection = (eDirection)direction;

		}
	}
	void KoopaScript::Move()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		PlayAnimationByDirection(mDirection);
		Translate(tr);

		if (mFireTime > 2.5f)
		{
			mFireTime = 0.0;
			FireLaunch();
		}

		mState = eState::Idle;

	}
	void KoopaScript::Jump()
	{
		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 velocity = rb->GetVelocity();

		PlayAnimationByDirection(mDirection);

		velocity.y -= 250;
		
		rb->SetGround(false);
		rb->SetVelocity(velocity);

		if (mFireTime > 2.5f)
		{
			mFireTime = 0.0;
			FireLaunch();
		}

		mState = eState::Idle;
	}

	void KoopaScript::FireLaunch()
	{
		if (mDirection == eDirection::Left)
		{
			mAnimator->PlayAnimation(L"AttackL");
		}
		if (mDirection == eDirection::Right)
		{
			mAnimator->PlayAnimation(L"AttackR");
		}

		MakeFire();
	}

	void KoopaScript::MakeFire()
	{
		Fire* fire = object::Instantiate<Fire>(enums::eLayerType::Obstacle,GetMouthPos());
		
		FireScript* fireScript = fire->AddComponent<FireScript>();
		BoxCollider2D* col = fire->AddComponent<BoxCollider2D>();
		Animator* fireAnimator = fire->AddComponent<Animator>();

		col->SetSize(Vector2(0.3f, 0.1f));
		col->SetOffset(Vector2(-15, -5));

		graphics::Texture* fireTex = Resources::Find<graphics::Texture>(L"FIRE");

		fireAnimator->CreateAnimation(L"FireL", fireTex, Vector2(127, 251), Vector2(29, 10), Vector2(0, 0), 0.1f, 2);
		fireAnimator->CreateAnimation(L"FireR", fireTex, Vector2(158, 251), Vector2(29, 10), Vector2(0, 0), 0.1f, 2);

		 if (mDirection == eDirection::Left)
		 {
			 fire->SetDirection(Fire::eDirection::Left);
			 fireAnimator->PlayAnimation(L"FireL");
		 }
		 if (mDirection == eDirection::Right)
		 {
			 fire->SetDirection(Fire::eDirection::Right);
			 fireAnimator->PlayAnimation(L"FireR");
		 }

		
	}

	Vector2 KoopaScript::GetMouthPos()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		BoxCollider2D* col = GetOwner()->GetComponent<BoxCollider2D>();

		Vector2 pos = tr->GetPosition();
		Vector2 offset = col->GetOffset();
		Vector2 size = col->GetSize();

		if (mDirection == eDirection::Left)
		{
			pos += offset;
			pos.y += 10;
		}
		if (mDirection == eDirection::Right)
		{
			pos += offset + Vector2(size.x, 0) + Vector2(29,0);
			pos.y += 10;
		}

		return mMouthPos = pos;
		
	}

	void KoopaScript::Die()
	{
	}
	void KoopaScript::PlayAnimationByDirection(eDirection direction)
	{
		if (direction == eDirection::Left)
		{
			mAnimator->PlayAnimation(L"WalkL");
		}
		if (direction == eDirection::Right)
		{
			mAnimator->PlayAnimation(L"WalkR");
		}
	}
	void KoopaScript::Translate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();

		if (mDirection == eDirection::Left)
		{
			pos.x -= 200 * Time::DeltaTime();
		}
		
		if (mDirection == eDirection::Right)
		{
			pos.x += 200 * Time::DeltaTime();
		}
		
		tr->SetPosition(pos);

	}
	float KoopaScript::DetermineLeftOrRightByVector(GameObject* obj)
	{
		return 0.0f;
	}
	void KoopaScript::GotHitByBullet(GameObject* bullet)
	{
	}
	void KoopaScript::PlayHurtAnimation()
	{
	}

	void KoopaScript::FollowingCharacterDirection()
	{

		if (mPlayer == nullptr)
			return;

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Transform* koopaTr = GetOwner()->GetComponent<Transform>();

		Vector2 playerPos = playerTr->GetPosition();
		Vector2 koopaPos = koopaTr->GetPosition();

		float distance = fabs(playerPos.x - koopaPos.x);

		if (distance < 100.0f)
		{
			float direction = playerPos.x - koopaPos.x;

			if (direction < 0)
			{
				mDirection = eDirection::Left;
			}
			else
			{
				mDirection = eDirection::Right;
			}
		}
	}
}