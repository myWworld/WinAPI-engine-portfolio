#include "METurtleScript.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METime.h"
#include "MEMonsters.h"
#include "MEBullet.h"
#include "MEPlayerScript.h"
#include "MEHeadScript.h"

namespace ME
{
	TurtleScript::TurtleScript()
		:mSpeed(35.0f)
		, mHp(100.0f)
		, mRespawnTime(0.0f)
		, mbIsDead(false)
		, mbIsRespawn(false)
		, mbIsHurtState(false)
		, mbIsShell(false)
		, mbIsTouchShell(false)
		,mMonsterCenterPos(Vector2::Zero)
		, mPlayerCenterPos(Vector2::Zero)
		, mHead(nullptr)
	{
		mShellSpeed = mSpeed;
	}
	TurtleScript::~TurtleScript()
	{
	}
	void TurtleScript::Initialize()
	{
	}
	void TurtleScript::Update()
	{
		if (mAnimator == nullptr)
			mAnimator = GetOwner()->GetComponent<Animator>();

		if (mbIsHurtState)
		{
			mbIsHurtState = false;
		}

		if (mHead != nullptr)
		{
			AdjustHeadAfterHurt();
		}

		switch (mState)
		{
		case ME::TurtleScript::eState::Idle:
			Idle();
			break;
		case ME::TurtleScript::eState::Walk:
			Move();
			break;
		case ME::TurtleScript::eState::Die:
			Die();
			break;
		case ME::TurtleScript::eState::Hurt:
			Hurt();
			break;
		case ME::TurtleScript::eState::Shell:
			Shell();
			break;
		}
	}
	void TurtleScript::LateUpdate()
	{
	}
	void TurtleScript::Render(HDC hdc)
	{
	}
	void TurtleScript::OnCollisionEnter(Collider* other)
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

				if (mbIsHurtState == false)
					GotHitByBullet(bullet);

				mHp -= 10;
			}
		}

		if (mbIsShell == true)
		{
			if (other->GetOwner()->GetLayerType() == enums::eLayerType::Block)
			{
				if (mDirection == eDirection::Left)
					mDirection = eDirection::Right;
				else if (mDirection == eDirection::Right)
					mDirection = eDirection::Left;
			}


			if (other->GetOwner()->GetLayerType() == enums::eLayerType::Player)
			{
				if (mbIsTouchShell == true)
				{


					if (DetermineLeftOrRightByVector(other->GetOwner()) < 0)
					{
						mDirection = eDirection::Right;
					}
					else
					{
						mDirection = eDirection::Left;
					}

				}
			}
		}
	}


	void TurtleScript::OnCollisionStay(Collider* other)
	{
	}
	void TurtleScript::OnCollisionExit(Collider* other)
	{
	}

	void TurtleScript::Respawn()
	{
	}
	void TurtleScript::Hurt()
	{

	}
	void TurtleScript::Idle()
	{
		mTime += Time::DeltaTime();


		if (mTime > 1.0f)
		{
			mState = eState::Walk;
			int direction = rand() % 3;
			mDirection = (eDirection)direction;

			PlayAnimationByDirection(mDirection);
			
			mTime = 0.0f;
		}
	}

	void TurtleScript::Move()
	{
		mTime += Time::DeltaTime();

		if (mDirection == eDirection::End || mTime > 2.0f)
		{
			mState = eState::Idle;
			if (mDirection == eDirection::Left)
				mAnimator->PlayAnimation(L"IdleL");
			if (mDirection == eDirection::Right)
				mAnimator->PlayAnimation(L"IdleR");
		}

		Transform* tr = GetOwner()->GetComponent<Transform>();

		Translate(tr);
	}

	void TurtleScript::Die()
	{
		GameObject::eState state = GetOwner()->GetState();

		if (state == GameObject::eState::Active && mbIsDead == false)
		{
			mbIsDead = true;
			mbIsRespawn = true;

			if (mHead != nullptr)
			{
				mHead->SetDeath();
				mHead = nullptr;
			}

			static_cast<Monsters*>(GetOwner())->SetMonsterState(Monsters::eState::Dead);

			mAnimator->PlayAnimation(L"DeadR", false);
			PlayerScript::PlusScore();
		}

		if (mAnimator->IsComplete())
			GetOwner()->SetDeath();

	}
	void TurtleScript::Shell()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		if (mbIsTouchShell == true)
		{
			Translate(tr);
		}
	}
	void TurtleScript::PlayAnimationByDirection(eDirection direction)
	{
		switch (direction)
		{
		case ME::TurtleScript::eDirection::Left:
			mAnimator->PlayAnimation(L"WalkL");
			break;
		case ME::TurtleScript::eDirection::Right:
			mAnimator->PlayAnimation(L"WalkR");
			break;
		case ME::TurtleScript::eDirection::End:
			break;
		default:
			break;
		}
	}
	void TurtleScript::Translate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();

		if (mHead != nullptr)
		{
			HeadTranslate(tr);
			return;
		}

		switch (mDirection)
		{
		case ME::TurtleScript::eDirection::Left:
			pos += Vector2::Left *( mSpeed * Time::DeltaTime());
			break;
		case ME::TurtleScript::eDirection::Right:
			pos += Vector2::Right * (mSpeed * Time::DeltaTime());
			break;
		}

		tr->SetPosition(pos);

	}

	void TurtleScript:: HeadTranslate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();

		Transform* headTr = mHead->GetComponent<Transform>();
		Vector2 HeadPos = headTr->GetPosition();

		switch (mDirection)
		{
		case ME::TurtleScript::eDirection::Left:
			pos += Vector2::Left * (mSpeed * Time::DeltaTime());
			HeadPos += Vector2::Left * (mSpeed * Time::DeltaTime());
			break;
		case ME::TurtleScript::eDirection::Right:
			pos += Vector2::Right * (mSpeed * Time::DeltaTime());
			HeadPos += Vector2::Right * (mSpeed * Time::DeltaTime());
			break;
		}

		tr->SetPosition(pos);
		headTr->SetPosition(HeadPos);

	}

	float TurtleScript::DetermineLeftOrRightByVector(GameObject* obj)
	{
		GetCenterPos(obj);
		Vector2 leftOrRight = mPlayerCenterPos - mMonsterCenterPos;

		return leftOrRight.x;
	}
	void TurtleScript::GotHitByBullet(GameObject* bullet)
	{
		Rigidbody* mushroomRb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 mushroomVelocity = mushroomRb->GetVelocity();

		Rigidbody* headRb = nullptr;
		Vector2 headVelocity = Vector2::Zero;

		if (mHead != nullptr)
		{
			headRb = mHead->GetComponent<Rigidbody>();
			headVelocity = headRb->GetVelocity();
		}

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


		if (headRb != nullptr)
			headRb->SetVelocity(headVelocity);
		
		mushroomRb->SetVelocity(mushroomVelocity);
		mushroomRb->SetGround(false);

		mbIsHurtState = true;
	//	PlayHurtAnimation();
	}

	void TurtleScript::PlayHurtAnimation()
	{

		switch (mDirection)
		{
		case ME::TurtleScript::eDirection::Left:
			mAnimator->PlayAnimation(L"WalkL");
			break;
		case ME::TurtleScript::eDirection::Right:
			mAnimator->PlayAnimation(L"WalkR");
			break;
		}
	}

	void TurtleScript::GetCenterPos(GameObject* player)
	{
		Transform* playerTr = player->GetComponent<Transform>();
		Transform* monsterTr = GetOwner()->GetComponent<Transform>();
	
		Vector2 playerColPos = playerTr->GetPosition() + player->GetComponent<Collider>()->GetOffset();
		Vector2 monsterColPos = monsterTr->GetPosition() + GetOwner()->GetComponent<Collider>()->GetOffset();

		Vector2 playerColliderSize = player->GetComponent<Collider>()->GetSize() * 100.0f;
		Vector2 monsterColliderSize = GetOwner()->GetComponent<Collider>()->GetSize() * 100.0f;

		mPlayerCenterPos = playerColPos + (playerColliderSize / 2.0f);
		mMonsterCenterPos = monsterColPos + (monsterColliderSize / 2.0f);
	
	}
	
	void TurtleScript::AdjustColForHeadAndBody()
	{
		Vector2 colOffset = GetOwner()->GetComponent<Collider>()->GetOffset();
		Vector2 colSize = GetOwner()->GetComponent<Collider>()->GetSize();

		colSize.y *= 0.5;
		colOffset.y += 6;
		GetOwner()->GetComponent<Collider>()->SetOffset(colOffset);
		GetOwner()->GetComponent<Collider>()->SetSize(colSize);

		if (mHead != nullptr)
		{
			Vector2	headColOffset = mHead->GetComponent<Collider>()->GetOffset();
			Vector2 headColSize = mHead->GetComponent<Collider>()->GetSize();
			headColSize.y *= 0.5;
			headColSize.x *= 2;

			headColOffset.x -= 7;
			headColOffset.y += 12;
			mHead->GetComponent<Collider>()->SetOffset(headColOffset);
			mHead->GetComponent<Collider>()->SetSize(headColSize);
		}

	}

	void TurtleScript::MakeTurtleIntoShell()
	{

		mState = eState::Shell;
		mSpeed = mSpeed * 4;

		if (mDirection == eDirection::Left)
			mAnimator->PlayAnimation(L"GotPressedL", false);
		if (mDirection == eDirection::Right)
			mAnimator->PlayAnimation(L"GotPressedR", false);

		AdjustColForHeadAndBody();

		mAnimator->PlayAnimation(L"ShellR");
	}


	void TurtleScript::AdjustHeadAfterHurt()
	{
		Collider* headCol = mHead->GetComponent<Collider>();
		Vector2 colOffset = headCol->GetOffset();

		Vector2 pos = GetOwner()->GetComponent<Transform>()->GetPosition();

		Transform* headTr = mHead->GetComponent<Transform>();

		Vector2 headPos = headTr->GetPosition();
		headPos.x = pos.x + colOffset.x - 7;

		headTr->SetPosition(headPos);
	}
}