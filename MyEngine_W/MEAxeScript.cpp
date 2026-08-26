#include "MEAxeScript.h"

namespace ME
{
	bool AxeScript::mbIsTrigger = false;

	AxeScript::AxeScript()
	{
	}
	AxeScript::~AxeScript()
	{
	}
	void AxeScript::Initialize()
	{
	}
	void AxeScript::Update()
	{
	}
	void AxeScript::LateUpdate()
	{
	}
	void AxeScript::Render(HDC hdc)
	{
	}
	void AxeScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			mbIsTrigger = true;
		}
	}
	void AxeScript::OnCollisionStay(Collider* other)
	{
	}
	void AxeScript::OnCollisionExit(Collider* other)
	{
	}
}