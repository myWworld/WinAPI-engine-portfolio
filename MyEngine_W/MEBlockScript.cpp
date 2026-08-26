#include "MEBlockScript.h"
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
#include "../MyEngine_Source/METime.h"

namespace ME
{


	ME::BlockScript::BlockScript()
		:mbIsOnBlock(false)
	{
	}

	ME::BlockScript::~BlockScript()
	{
	}

	void ME::BlockScript::Initialize()
	{
	}

	void ME::BlockScript::Update()
	{
	}

	void ME::BlockScript::LateUpdate()
	{
	}

	void ME::BlockScript::Render(HDC hdc)
	{
	}

	void ME::BlockScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Cannon")
			return;

		CannotPass(other);

	}

	void ME::BlockScript::OnCollisionStay(Collider* other)
	{
	}

	void ME::BlockScript::OnCollisionExit(Collider* other)
	{
		Rigidbody* rb = other->GetOwner()->GetComponent<Rigidbody>();

		if (mbIsOnBlock == true)
		{
			rb->SetGround(false);
			mbIsOnBlock = false;
		}
	}

	void BlockScript::CannotPass(Collider *obj)
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

		float playerBottom = playerCenterPos.y + (playerColSize.y / 2.0f);
		float chimneyTop = ChimneyCenterPos.y - (ChimneyColSize.y / 2.0f);

		float overlapX = (playerColSize.x / 2.0f + ChimneyColSize.x / 2.0f) - fabs(playerCenterPos.x - ChimneyCenterPos.x);
		float overlapY = (playerColSize.y / 2.0f + ChimneyColSize.y / 2.0f) - fabs(playerCenterPos.y - ChimneyCenterPos.y);

		// 두 축 모두 겹쳐 있어야만 실제 충돌로 인정
		if (overlapX > 0 && overlapY > 0)
		{
			Vector2 playerPos = playerTr->GetPosition();

			//  Y축 겹침이 더 적다 = 위나 아래에서 부딪혔다
			if (overlapX > overlapY)
			{
				// 플레이어의 중심이 블록 중심보다 위에 있다 = 위에서 
				if (playerCenterPos.y < ChimneyCenterPos.y)
				{
					playerPos.y -= overlapY - 1.0f; // 겹친 만큼 위로 밀어냄 (-1.0f는 진동 방지 오차)
					playerRb->SetGround(true);
					mbIsOnBlock = true;
				}
				// 플레이어의 중심이 블록 중심보다 아래에 있다 = 밑에서
				else
				{
					playerPos.y += overlapY - 1.0f; // 겹친 만큼 아래로 튕겨냄
					playerRb->SetVelocity(Vector2(playerRb->GetVelocity().x, 0.0f)); // 올라가던 관성 제거
				}
			}
			// X축 겹침이 더 적다 = 왼쪽이나 오른쪽 옆구리에 부딪혔다
			else
			{
				if (playerCenterPos.x < ChimneyCenterPos.x) // 플레이어가 블록의 왼쪽에 있음
				{
					playerPos.x -= overlapX - 1.0f; // 왼쪽으로 밀어냄
				}
				else // 플레이어가 블록의 오른쪽에 있음
				{
					playerPos.x += overlapX - 1.0f; // 오른쪽으로 밀어냄
				}

				// 옆에서 박았을 때 벽에 붙어서 안 떨어지는 현상을 막기위해 X축 속도를 0으로 만들어 줍니다.
				playerRb->SetVelocity(Vector2(0.0f, playerRb->GetVelocity().y));
			}

			// 최종 좌표 적용
			playerTr->SetPosition(playerPos);
		}

		//float yVelocity = playerRb->GetVelocity().y;
		//float dynamicTolerance = 8.0f;
		//if (yVelocity > 0) // 아래로 떨어지고 있을 때만
		//{
		//	dynamicTolerance += (yVelocity * Time::DeltaTime());
		//}

		//// 동적 오차 범위를 적용하여 위에서 밟았는지 정확히 판정
		//if (fabs(playerBottom - chimneyTop) <= dynamicTolerance)
		//{
		//	float len = fabs(playerCenterPos.y - ChimneyCenterPos.y);
		//	float scale = fabs(playerColSize.y / 2.0f + ChimneyColSize.y / 2.0f);

		//	if (len < scale)
		//	{
		//		Vector2 playerPos = playerTr->GetPosition();
		//		playerPos.y -= (scale - len) - 1.0f;

		//		playerTr->SetPosition(playerPos);
		//		mbIsOnBlock = true;
		//	}


		//	playerRb->SetGround(true);

		//}
		//else
		//{
		//	float direct = playerCenterPos.x - ChimneyCenterPos.x;
		//	Vector2 playerPos = playerTr->GetPosition();

		//	if (direct <= 0)
		//	{
		//		playerPos.x -= 3.0f;
		//	}
		//	else
		//	{
		//		playerPos.x += 3.0f;

		//	}

		//	playerTr->SetPosition(playerPos);
		//}

	}

}