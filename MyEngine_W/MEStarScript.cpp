#include "MEStarScript.h"
#include "MEGameObject.h"

#include "MEPlayerScript.h"

namespace ME
{
	StarScript::StarScript()
	{
	}
	StarScript::~StarScript()
	{
	}
	void StarScript::Initialize()
	{
	}
	void StarScript::Update()
	{
	}
	void StarScript::LateUpdate()
	{
	}
	void StarScript::Render(HDC hdc)
	{
	}
	void StarScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			GameObject* star = GetOwner();
			star->SetActive(false);
			star->SetDeath();

			PlayerScript::PlusScore();
		}
	}
	void StarScript::OnCollisionStay(Collider* other)
	{
	}
	void StarScript::OnCollisionExit(Collider* other)
	{
	}
}