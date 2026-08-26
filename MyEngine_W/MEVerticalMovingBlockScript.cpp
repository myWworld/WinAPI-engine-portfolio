#include "MEVerticalMovingBlockScript.h"
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
	VerticalMovingBlockScript::VerticalMovingBlockScript()
		:mDirection(Direction::Up)
		,mbIsOnBlock(false)
	{
	}
	VerticalMovingBlockScript::~VerticalMovingBlockScript()
	{
	}
	void VerticalMovingBlockScript::Initialize()
	{
	}
	void VerticalMovingBlockScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (pos.y <= mMaxHeight)
		{
			mDirection = Direction::Down;
		}

		if (pos.y >= mMinHeight)
		{
			mDirection = Direction::Up;
		}

		if (mDirection == Direction::Down)
		{
			pos.y += 15 * Time::DeltaTime();
		}
		else if (mDirection == Direction::Up)
		{
			pos.y -= 15 * Time::DeltaTime();
		}

		tr->SetPosition(pos);
	}
	void VerticalMovingBlockScript::LateUpdate()
	{
	}
	void VerticalMovingBlockScript::Render(HDC hdc)
	{
	}
	void VerticalMovingBlockScript::OnCollisionEnter(Collider* other)
	{
	

		if (other->GetName() == L"Player")
		{
			mbIsOnBlock = true;
			CannotPass(other);
		}

	}
	void VerticalMovingBlockScript::OnCollisionStay(Collider* other)
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
				if (mDirection == Direction::Down)
				{
					playerPos.y += 15 * Time::DeltaTime();

				}
				else if (mDirection == Direction::Up)
				{
					playerPos.y -= 15 * Time::DeltaTime();
				}

				playerTr->SetPosition(playerPos);

				CannotPass(other);
			}
		}

	
		
	}
	void VerticalMovingBlockScript::OnCollisionExit(Collider* other)
	{
		mbIsOnBlock = false;
		Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();

		playerRb->SetGround(false);
	}

	void VerticalMovingBlockScript::CannotPass(Collider* obj)
	{
		Rigidbody* playerRb = obj->GetOwner()->GetComponent<Rigidbody>();
		Transform* playerTr = obj->GetOwner()->GetComponent<Transform>();
		Collider* playerCol = obj;

		Transform* ChimneyTr = this->GetOwner()->GetComponent<Transform>();
		Vector2 ChimneyPos = ChimneyTr->GetPosition();

		Rigidbody* ChimneyRb = this->GetOwner()->GetComponent<Rigidbody>();
		Collider* ChimneyCol = this->GetOwner()->GetComponent<Collider>();


		Vector2 playerColSize = playerCol->GetSize() * 100.0f;
		Vector2 ChimneyColSize = ChimneyCol->GetSize() * 100.0f;

		Vector2 playerCenterPos = (playerTr->GetPosition() + playerCol->GetOffset()) + playerColSize / 2.0f;
		Vector2 ChimneyCenterPos = (ChimneyTr->GetPosition() + ChimneyCol->GetOffset()) + ChimneyColSize / 2.0f;

		float len = fabs(playerCenterPos.y - ChimneyCenterPos.y);
		float scale = fabs(playerColSize.y / 2.0f + ChimneyColSize.y / 2.0f);

		if (len < scale)
		{
			Vector2 playerPos = playerTr->GetPosition();
			playerPos.y -= (scale - len) - 1.0f;

			playerTr->SetPosition(playerPos);

		}


		playerRb->SetGround(true);

		
	}
}