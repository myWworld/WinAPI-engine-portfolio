#include "MEFloorScript.h"
#include "MERigidbody.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MECollider.h"
#include "MEInput.h"
#include "../MyEngine_Source/METime.h"

namespace ME
{
	FloorScript::FloorScript()
		:mbIsOnFloor(false)
	{
	}
	FloorScript::~FloorScript()
	{
	}
	void FloorScript::Initialize()
	{
	}
	void FloorScript::Update()
	{

	}
	void FloorScript::LateUpdate()
	{
	}
	void FloorScript::Render(HDC hdc)
	{
	}
	void FloorScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetOwner()->GetLayerType() == enums::eLayerType::Player
			 || other->GetOwner()->GetLayerType() == enums::eLayerType::Items
			   || other->GetOwner()->GetLayerType() == enums::eLayerType::Monster)
		{
			CannotPass(other);
		}
	}
	void FloorScript::OnCollisionStay(Collider* other)
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
	void FloorScript::OnCollisionExit(Collider* other)
	{
		Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();
		mbIsOnFloor = false;
		playerRb->SetGround(false);
	}

	void FloorScript::CannotPass(Collider* obj)
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

		float overlapX = (playerColSize.x / 2.0f + floorColSize.x / 2.0f) - fabs(playerCenterPos.x - floorCenterPos.x);
		float overlapY = (playerColSize.y / 2.0f + floorColSize.y / 2.0f) - fabs(playerCenterPos.y - floorCenterPos.y);

		// 실제로 충돌(겹침)이 발생했는지 확인
		if (overlapX > 0 && overlapY > 0)
		{

			// Y축으로 더 적게 겹쳤고(위/아래 충돌), 플레이어가 바닥보다 위에 있을 때 (위에서 밟았을 때)
			if (overlapX > overlapY && playerCenterPos.y < floorCenterPos.y)
			{
				Vector2 playerPos = playerTr->GetPosition();
				playerPos.y -= overlapY - 1.0f; // 겹친 만큼 정확히 위로 밀어냄 (-1.0f는 진동 방지)
				playerTr->SetPosition(playerPos);

				mbIsOnFloor = true;
				playerRb->SetGround(true);
			}
		}
	}
}