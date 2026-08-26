#include "MECannoScript.h"
#include "MERigidbody.h"
#include "METransform.h"
#include "METime.h"

#include "MEPlayerScript.h"

namespace ME
{
	CannonScript::CannonScript()
		:mbIsBumpedWhenStarIsOn(false)
	{
	}
	CannonScript::~CannonScript()
	{
	}
	void CannonScript::Initialize()
	{
	}
	void CannonScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (mbIsBumpedWhenStarIsOn == true)
		{
			pos.x -= 100 * Time::DeltaTime();
			pos.y -= 100 * Time::DeltaTime();
			tr->SetRotation(20);
		}
		else
			pos.x -= 100 * Time::DeltaTime();

		tr->SetPosition(pos);

		if (pos.x < 0)
		{
			GetOwner()->SetDeath();
		}
	}
	void CannonScript::LateUpdate()
	{
	}
	void CannonScript::Render(HDC hdc)
	{
	}
	void CannonScript::Destroy()
	{
	}
	void CannonScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			if (PlayerScript::IsStar() == true)
			{
				mbIsBumpedWhenStarIsOn = true;
				return;
			}

			if (FlagScript::IsOnFlag() == true || PlayerScript::IsHurtState())
			{
				return;
			}

			GameObject* player = other->GetOwner();

			Rigidbody *playerRb = player->GetComponent<Rigidbody>();

			Vector2 velocity = playerRb->GetVelocity();
			velocity.y = -500.0f;

			playerRb->SetVelocity(velocity);

			playerRb->SetGround(false);
		}
	}

	void CannonScript::OnCollisionStay(Collider* other)
	{
	}
	void CannonScript::OnCollisionExit(Collider* other)
	{
	}
}