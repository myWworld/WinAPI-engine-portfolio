#include "MEHeadScript.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METime.h"
#include "MEMonsters.h"
#include "MEBullet.h"
#include "MEPlayerScript.h"
#include "METransform.h"

#include "METurtleScript.h"

namespace ME
{
	HeadScript::HeadScript()
		:mbIsHead(false)
		,mHeadOwner(nullptr)
	{
	}
	HeadScript::~HeadScript()
	{
	}
	void HeadScript::Initialize()
	{
	}
	void HeadScript::Update()
	{
	}
	void HeadScript::LateUpdate()
	{
	}
	void HeadScript::Render(HDC hdc)
	{
	}
	void HeadScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			if (mHeadOwner == nullptr)
				return;

			if (mHeadOwner->GetComponent<Collider>()->GetName() == L"Turtle")
			{
				TurtleScript* turtleScript = mHeadOwner->GetComponent<TurtleScript>();
				
				if (turtleScript->IsShell() == false)
				{
					turtleScript->SetIsShell(true);
					turtleScript->MakeTurtleIntoShell();
				}
				else
				{
					if (turtleScript->IsTouchedShell() == false)
					{
						turtleScript->SetTouchedShell(true);
						turtleScript->MakeHeadNull();
						GetOwner()->SetDeath();
					}
				}

				MakePlayerBounce(other);
			}
		}
	}
	void HeadScript::OnCollisionStay(Collider* other)
	{
	}
	void HeadScript::OnCollisionExit(Collider* other)
	{
	}

	void HeadScript::CannotPass(Collider* obj)
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

		float len = fabs(playerCenterPos.y - floorCenterPos.y);
		float scale = fabs(playerColSize.y / 2.0f + floorColSize.y / 2.0f);

		if (len < scale)
		{
			Vector2 playerPos = playerTr->GetPosition();
			playerPos.y -= (scale - len) - 1.0f;
			playerTr->SetPosition(playerPos);
		}


		playerRb->SetGround(true);
	}

	void HeadScript::MakePlayerBounce(Collider* player)
	{
		Rigidbody* rb = player->GetOwner()->GetComponent<Rigidbody>();

		Vector2 velocity = rb->GetVelocity();
		velocity.y = -300.0f;
		rb->SetVelocity(velocity);
		rb->SetGround(false);
	}

}