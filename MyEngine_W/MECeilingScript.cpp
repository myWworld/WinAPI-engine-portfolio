#include "MECeilingScript.h"

#include "MERigidbody.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MECollider.h"
#include "MEInput.h"


namespace ME
{
	CeilingScript::CeilingScript()
	{
	}
	CeilingScript::~CeilingScript()
	{
	}
	void CeilingScript::Initialize()
	{
	}
	void CeilingScript::Update()
	{
	}
	void CeilingScript::LateUpdate()
	{
	}
	void CeilingScript::Render(HDC hdc)
	{
	}
	void CeilingScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			CannotPass(other);
		}
	}
	void CeilingScript::OnCollisionStay(Collider* other)
	{
	}
	void CeilingScript::OnCollisionExit(Collider* other)
	{
	//Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();
	//playerRb->StopMoving();
	}

	void CeilingScript::CannotPass(Collider* obj)
	{
		Rigidbody* playerRb = obj->GetOwner()->GetComponent<Rigidbody>();
		Transform* playerTr = obj->GetOwner()->GetComponent<Transform>();
		Collider* playerCol = obj;

		Transform* ceilingTr = this->GetOwner()->GetComponent<Transform>();
		Rigidbody* ceilingRb = this->GetOwner()->GetComponent<Rigidbody>();
		Collider* ceilingCol = this->GetOwner()->GetComponent<Collider>();


		Vector2 playerColSize = playerCol->GetSize() * 100.0f;
		Vector2 ceilingColSize = ceilingCol->GetSize() * 100.0f;

		Vector2 playerCenterPos = (playerTr->GetPosition() + playerCol->GetOffset()) + playerColSize / 2.0f;
		Vector2 ceilingCenterPos = (ceilingTr->GetPosition() + ceilingCol->GetOffset()) + ceilingColSize / 2.0f;

		float ceilingBottom = ceilingCenterPos.y + (ceilingColSize.y / 2.0f);
		float playerBottom = playerCenterPos.y + (playerColSize.y / 2.0f);

		float playerTop = playerCenterPos.y - (playerColSize.y / 2.0f);

		Vector2 velocity = playerRb->GetVelocity();
		Vector2 playerPos = playerTr->GetPosition();

		if (playerTop >= (ceilingBottom - 5.0f))
		{
			float len = fabs(playerCenterPos.y - ceilingCenterPos.y);
			float scale = fabs(playerColSize.y / 2.0f + ceilingColSize.y / 2.0f);
			
			if (len < scale)
			{
				playerPos.y += (scale - len) + 1.0f;
				playerTr->SetPosition(playerPos);

				velocity.y += 150.0f;
			}
			playerRb->SetVelocity(velocity);
		}
		else
		{
			float direct = playerCenterPos.x - ceilingCenterPos.x;
			

			if (direct <= 0)
			{
				playerPos.x -= 2.0f;
			}
			else
			{
				playerPos.x += 2.0f;

			}

			playerTr->SetPosition(playerPos);
		}

		

	}

}