#include "MEOnlyDownBlockScript.h"

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
	OnlyMovingDownScript::OnlyMovingDownScript()
		:mbIsOnBlock(false)
	{
	}
	OnlyMovingDownScript::~OnlyMovingDownScript()
	{
	}
	void OnlyMovingDownScript::Initialize()
	{
	}
	void OnlyMovingDownScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (mbIsOnBlock == true)
		{
			pos.y += 130 * Time::DeltaTime();
			tr->SetPosition(pos);
		}

	}
	void OnlyMovingDownScript::LateUpdate()
	{
	}
	void OnlyMovingDownScript::Render(HDC hdc)
	{
	}
	void OnlyMovingDownScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			CannotPass(other);
			
		}
	}
	void OnlyMovingDownScript::OnCollisionStay(Collider* other)
	{

		if (other->GetName() == L"Player")
		{
			if (Input::GetKey(eKeyCode::Space))
			{
				mbIsOnBlock = false;
			}

			if(mbIsOnBlock ==true)
				CannotPass(other);


		}

	}
	void OnlyMovingDownScript::OnCollisionExit(Collider* other)
	{

		if (other->GetName() == L"Player")
		{
			Rigidbody* rb = other->GetOwner()->GetComponent<Rigidbody>();
			rb->SetGround(false);

			mbIsOnBlock = false;

		}

	}
	void OnlyMovingDownScript::CannotPass(Collider* obj)
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