#include "MEHorizonMovingBlockScript.h"

#include "MEResource.h"
#include "MEScenes.h"
#include "MEInput.h"
#include "MEGameOverScene.h"
#include "MESceneManager.h"
#include "METransform.h"
#include "CommonInclude.h"
#include "MECollider.h"
#include "MEBoxCollider2D.h"
#include "MERigidbody.h"
#include "METime.h"

namespace ME
{
	HorizonMovingBlockScript::HorizonMovingBlockScript()
		:mDirection(Direction::Right)
		, mObjectIsOnBlock(nullptr)
	{
	}
	HorizonMovingBlockScript::~HorizonMovingBlockScript()
	{
	}
	void HorizonMovingBlockScript::Initialize()
	{
	}
	void HorizonMovingBlockScript::Update()
	{
		MovingBlockByItsDirection();
		MovingPlayerByIsOnBlock();

	}
	void HorizonMovingBlockScript::LateUpdate()
	{
	}
	void HorizonMovingBlockScript::Render(HDC hdc)
	{
	}
	void HorizonMovingBlockScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			mbIsOnBlock = true;
			mObjectIsOnBlock = other->GetOwner();
			CannotPass(other);
		}

	}
	void HorizonMovingBlockScript::OnCollisionStay(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			Transform* playerTr = other->GetOwner()->GetComponent<Transform>();
			Vector2 playerPos = playerTr->GetPosition();


			if (Input::GetKey(eKeyCode::Space))
			{
				mbIsOnBlock = false;
			}

			if (mbIsOnBlock == true)
			{
				CannotPass(other);
			}
		}
	}
	void HorizonMovingBlockScript::OnCollisionExit(Collider* other)
	{
		mbIsOnBlock = false;
		Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();

		playerRb->SetGround(false);

		if (mObjectIsOnBlock == nullptr) return;

		if (other->GetName() == mObjectIsOnBlock->GetComponent<Rigidbody>()->GetName())
			mObjectIsOnBlock = nullptr;
	}

	void HorizonMovingBlockScript::CannotPass(Collider* obj)
	{
		Rigidbody* playerRb = obj->GetOwner()->GetComponent<Rigidbody>();
		Transform* playerTr = obj->GetOwner()->GetComponent<Transform>();
		Collider* playerCol = obj;

		Transform* blockTr = this->GetOwner()->GetComponent<Transform>();
		Vector2 blockPos = blockTr->GetPosition();

		Rigidbody* blockRb = this->GetOwner()->GetComponent<Rigidbody>();
		Collider* blockCol = this->GetOwner()->GetComponent<Collider>();


		Vector2 playerColSize = playerCol->GetSize() * 100.0f;
		Vector2 blockColSize = blockCol->GetSize() * 100.0f;

		Vector2 playerCenterPos = (playerTr->GetPosition() + playerCol->GetOffset()) + playerColSize / 2.0f;
		Vector2 blockCenterPos = (blockTr->GetPosition() + blockCol->GetOffset()) + blockColSize / 2.0f;

		float len = fabs(playerCenterPos.y - blockCenterPos.y);
		float scale = fabs(playerColSize.y / 2.0f + blockColSize.y / 2.0f);

		if (len < scale)
		{
			Vector2 playerPos = playerTr->GetPosition();
			playerPos.y -= (scale - len) - 1.0f;

			playerTr->SetPosition(playerPos);

		}


		playerRb->SetGround(true);


	}

	void HorizonMovingBlockScript::MovingBlockByItsDirection()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (pos.x > mMaxLength)
		{
			mDirection = Direction::Left;
		}

		if (pos.x < mMinLength)
		{
			mDirection = Direction::Right;
		}

		if (mDirection == Direction::Right)
		{
			pos.x += 30 * Time::DeltaTime();
		}
		else if (mDirection == Direction::Left)
		{
			pos.x -= 30 * Time::DeltaTime();
		}

		tr->SetPosition(pos);
	}
	void HorizonMovingBlockScript::MovingPlayerByIsOnBlock()
	{
		if (mObjectIsOnBlock == nullptr)
			return;

		Transform* playerTr = mObjectIsOnBlock->GetComponent<Transform>();
		Vector2 playerPos = playerTr->GetPosition();

		if (mbIsOnBlock == true)
		{
			if (mDirection == Direction::Right)
			{
				playerPos.x += 30 * Time::DeltaTime();

			}
			else if (mDirection == Direction::Left)
			{
				playerPos.x -= 30 * Time::DeltaTime();
			}

			playerTr->SetPosition(playerPos);
		}
	}
}