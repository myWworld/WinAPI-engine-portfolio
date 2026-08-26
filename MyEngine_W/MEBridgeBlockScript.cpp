#include "MEBridgeBlockScript.h"
#include "MERigidbody.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MECollider.h"
#include "MEInput.h"
#include "METime.h"
#include "MEAxeScript.h"

namespace ME
{
	BridgeBlockScript::BridgeBlockScript()
		:mbIsOnFloor(false)
		, mbIsTriggered(false)
		,mVanishgTime(0.0f)
	{
	}
	BridgeBlockScript::~BridgeBlockScript()
	{
	}
	void BridgeBlockScript::Initialize()
	{
	}
	void BridgeBlockScript::Update()
	{
		mbIsTriggered = AxeScript::IsTrigger();

		if (mbIsTriggered)
		{
			mVanishgTime += Time::DeltaTime();

			if (mVanishgTime > mVanishgInterval)
			{
				GetOwner()->SetActive(false);
				GetOwner()->SetDeath();
			}
		}

	}
	void BridgeBlockScript::LateUpdate()
	{
	}
	void BridgeBlockScript::Render(HDC hdc)
	{
	}
	void BridgeBlockScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetOwner()->GetLayerType() == enums::eLayerType::Player
			|| other->GetOwner()->GetLayerType() == enums::eLayerType::Items
			|| other->GetOwner()->GetLayerType() == enums::eLayerType::Monster)
		{
			CannotPass(other);
		}
	}
	void BridgeBlockScript::OnCollisionStay(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();

			if (Input::GetKey(eKeyCode::Space))
				mbIsOnFloor = false;
		}
		

		if (mbIsOnFloor == true)
			CannotPass(other);
	}
	void BridgeBlockScript::OnCollisionExit(Collider* other)
	{
		Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();
		mbIsOnFloor = false;
		playerRb->SetGround(false);
	}
	void BridgeBlockScript::CannotPass(Collider* obj)
	{
		Rigidbody* playerRb = obj->GetOwner()->GetComponent<Rigidbody>();
		Transform* playerTr = obj->GetOwner()->GetComponent<Transform>();
		Collider* playerCol = obj;

		Transform* floorTr = this->GetOwner()->GetComponent<Transform>();
		Rigidbody* floorRb = this->GetOwner()->GetComponent<Rigidbody>();
		Collider* floorCol = this->GetOwner()->GetComponent<Collider>();


		Vector2 playerColSize = playerCol->GetSize() * 100.0f;
		Vector2 floorColSize = floorCol->GetSize() * 100.0f;

		Vector2 playerCenterPos = (playerTr->GetPosition() + playerCol->GetOffset()) + playerColSize / 2.0f;
		Vector2 floorCenterPos = (floorTr->GetPosition() + floorCol->GetOffset()) + floorColSize / 2.0f;

		float floorTop = floorCenterPos.y - (floorColSize.y / 2.0f);

		if (playerCenterPos.y > floorTop)
			return;


		float len = fabs(playerCenterPos.y - floorCenterPos.y);
		float scale = fabs(playerColSize.y / 2.0f + floorColSize.y / 2.0f);

		if (len < scale)
		{
			Vector2 playerPos = playerTr->GetPosition();
			playerPos.y -= (scale - len) - 1.0f;

			mbIsOnFloor = true;
			playerTr->SetPosition(playerPos);
		}


		playerRb->SetGround(true);
	}

}