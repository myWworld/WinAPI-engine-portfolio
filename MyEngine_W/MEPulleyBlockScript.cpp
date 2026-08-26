#include "MEPulleyBlockScript.h"

#include "MEAnimator.h"
#include "MEGameObject.h"
#include "MEBoxCollider2D.h"
#include "METime.h"
#include "MERigidbody.h"
#include "METransform.h"
#include "MEInput.h"


namespace ME
{
	ME::PulleyBlockScript::PulleyBlockScript()
		:mbIsOnBlock(false)
	{
	}
	PulleyBlockScript::~PulleyBlockScript()
	{
	}
	void PulleyBlockScript::Initialize()
	{
	}
	void PulleyBlockScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();


		Transform* otherTr = mOtherBlock->GetComponent<Transform>();
		Vector2 otherPos = otherTr->GetPosition();
		Rigidbody* otherRb = mOtherBlock->GetComponent<Rigidbody>();

	

		if (mbIsOnBlock == true)
		{
			if (mMinHeight > pos.y)
			{
				//pos.y += 140 * Time::DeltaTime();
				//otherPos.y -= 140 * Time::DeltaTime();
				//
				//tr->SetPosition(pos);
				//otherTr->SetPosition(otherPos);

				rb->AddForce(Vector2(0, 100.0f));
				otherRb->AddForce(Vector2(0, -100.0f));
			}
			else
			{
				rb->StopMoving();
				otherRb->StopMoving();
			}
		}

		if (pos.y >= mMinHeight)
			pos.y = mMinHeight;
		if (otherPos.y <= mMaxHeight)
			otherPos.y = mMaxHeight;

		tr->SetPosition(pos);
		otherTr->SetPosition(otherPos);

		
	}
	void PulleyBlockScript::LateUpdate()
	{
	}
	void PulleyBlockScript::Render(HDC hdc)
	{
	}
	void PulleyBlockScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			CannotPass(other);

		}
	}
	void PulleyBlockScript::OnCollisionStay(Collider* other)
	{
		
		if (Input::GetKey(eKeyCode::Space))
		{
			mbIsOnBlock = false;
		}

		if(mbIsOnBlock== true)
			CannotPass(other);
	}
	void PulleyBlockScript::OnCollisionExit(Collider* other)
	{
		Rigidbody* rb = other->GetOwner()->GetComponent<Rigidbody>();

		rb->SetGround(false);
		mbIsOnBlock = false;
	}
	void PulleyBlockScript::CannotPass(Collider* obj)
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
			mbIsOnBlock = true;

			playerTr->SetPosition(playerPos);

		}

		playerRb->SetGround(true);


	}

}