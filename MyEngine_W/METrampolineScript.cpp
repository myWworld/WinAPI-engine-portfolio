#include "METrampolineScript.h"
#include "MEAnimator.h"
#include "MEGameObject.h"
#include "MEBoxCollider2D.h"
#include "METime.h"
#include "MERigidbody.h"
#include "METransform.h"
#include "MEInput.h"

namespace ME
{
	TrampolineScript::TrampolineScript()
		:mbIsOnTrampoline(false)
		,minHeight()
	{
	}
	TrampolineScript::~TrampolineScript()
	{
	}
	void TrampolineScript::Initialize()
	{
	}
	void TrampolineScript::Update()
	{
		BoxCollider2D* tramCol = GetOwner()->GetComponent<BoxCollider2D>();
		Vector2 tramColOffset = tramCol->GetOffset();

		if (mbIsOnTrampoline == true)
		{
			tramColOffset.y += 16 *Time::DeltaTime();
		}
		else
		{
			tramColOffset.y = originOffsetY;
		}

		tramCol->SetOffset(tramColOffset);

	}
	void TrampolineScript::LateUpdate()
	{
	}
	void TrampolineScript::Render(HDC hdc)
	{
	}
	void TrampolineScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Cannon"
			|| other->GetName() == L"Bullet")
			return;
		
		if (other->GetName() == L"Player")
		{
			Animator* tramAnimator = GetOwner()->GetComponent<Animator>();
				
			CannotPass(other);
		

			if(mbIsOnTrampoline == true)
				tramAnimator->PlayAnimation(L"PressedR", false);
		
		
		}
	}

	void TrampolineScript::OnCollisionStay(Collider* other)
	{

		if (other->GetName() == L"Player")
		{
			Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();
			Animator* tramAnimator = GetOwner()->GetComponent<Animator>();

			if (mbIsOnTrampoline == true)
			{
				CannotPass(other);

				if (tramAnimator->IsComplete() && mbIsOnTrampoline == true)
				{
					Vector2 playerVel = playerRb->GetVelocity();
					playerVel.y -= 136;

					playerRb->SetGround(false);
					playerRb->SetVelocity(playerVel);
				}


				if (Input::GetKey(eKeyCode::Space))
				{
					mbIsOnTrampoline = false;
				}
			}
			
			
		}
	}

	void TrampolineScript::OnCollisionExit(Collider* other)
	{

		if (other->GetName() == L"Player")
		{
			Animator* tramAnimator = GetOwner()->GetComponent<Animator>();
			Rigidbody* playerRb = other->GetOwner()->GetComponent<Rigidbody>();
			
			if (mbIsOnTrampoline == true)
			{
				mbIsOnTrampoline = false;
				playerRb->SetGround(false);
			}
			tramAnimator->PlayAnimation(L"IdleR");
		}
	}

	void TrampolineScript::CannotPass(Collider* obj)
	{
		Rigidbody* playerRb = obj->GetOwner()->GetComponent<Rigidbody>();
		Transform* playerTr = obj->GetOwner()->GetComponent<Transform>();
		Collider* playerCol = obj;

		Transform* tramTr = this->GetOwner()->GetComponent<Transform>();
		Vector2 tramPos = tramTr->GetPosition();

		Rigidbody* tramRb = this->GetOwner()->GetComponent<Rigidbody>();
		Collider* tramCol = this->GetOwner()->GetComponent<Collider>();


		Vector2 playerColSize = playerCol->GetSize() * 100.0f;
		Vector2 tramColSize = tramCol->GetSize() * 100.0f;

		Vector2 playerCenterPos = (playerTr->GetPosition() + playerCol->GetOffset()) + playerColSize / 2.0f;
		Vector2 tramCenterPos = (tramTr->GetPosition() + tramCol->GetOffset()) + tramColSize / 2.0f;

		float playerBottom = playerCenterPos.y + (playerColSize.y / 2.0f);
		float tramTop = tramCenterPos.y - (tramColSize.y / 2.0f);

		if (fabs(playerBottom - tramTop) <= 3)
		{
			float len = fabs(playerCenterPos.y - tramCenterPos.y);
			float scale = fabs(playerColSize.y / 2.0f + tramColSize.y / 2.0f);

			if (len < scale)
			{
				Vector2 playerPos = playerTr->GetPosition();
				playerPos.y -= (scale - len)- 1.0f;
				mbIsOnTrampoline = true;

				playerTr->SetPosition(playerPos);
	
			}


			playerRb->SetGround(true);

		}
		else
		{
			float direct = playerCenterPos.x - tramCenterPos.x;
			Vector2 playerPos = playerTr->GetPosition();

			if (direct <= 0)
			{
				playerPos.x -= 3.0f;
			}
			else
			{
				playerPos.x += 3.0f;

			}
			playerTr->SetPosition(playerPos);
		}

	}
}