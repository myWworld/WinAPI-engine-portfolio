#include "MEFireScript.h"
#include "CommonInclude.h"
#include "MEInput.h"
#include "METransform.h"
#include "METime.h"
#include "MEResources.h"
#include "MEAnimator.h"
#include "MEFire.h"

 


namespace ME
{
	FireScript::FireScript()
		:mTime(0.0f)
	{
	}
	FireScript::~FireScript()
	{
	}
	void FireScript::Initialize()
	{
	}
	void FireScript::Update()
	{
		Fire* fire = static_cast<Fire*>(GetOwner());

		mTime += Time::DeltaTime();

		if (mTime > 4.0f)
		{
			fire->SetActive(false);
			fire->SetDeath();
		}

		MoveFireByDirection();
	}


	void FireScript::LateUpdate()
	{
	}
	void FireScript::Render(HDC hdc)
	{
	}
	void FireScript::Destroy()
	{
	}
	void FireScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			GetOwner()->SetActive(false);
			GetOwner()->SetDeath();
		}
	}
	void FireScript::OnCollisionStay(Collider* other)
	{
	}
	void FireScript::OnCollisionExit(Collider* other)
	{
	}

	void FireScript::MoveFireByDirection()
	{

		Fire* fire = static_cast<Fire*>(GetOwner());

		Transform* tr = fire->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (fire->GetDirection() == Fire::eDirection::Left)
		{
			pos.x -= 150.0f * Time::DeltaTime();	
		}
		else if (fire->GetDirection() == Fire::eDirection::Right)
		{
			pos.x += 150.0f * Time::DeltaTime();
		}


		tr->SetPosition(pos);
	}
}