#include "MELastDoorScript.h"
#include "MESceneManager.h"
#include "MEAnimator.h"

namespace ME
{
	bool LastDoorScript::mbIsClear = false;

	LastDoorScript::LastDoorScript()
	{
	}
	LastDoorScript::~LastDoorScript()
	{
		
	}
	void LastDoorScript::Initialize()
	{
	}
	void LastDoorScript::Update()
	{
	}
	void LastDoorScript::LateUpdate()
	{
	}
	void LastDoorScript::Render(HDC hdc)
	{
	}
	void LastDoorScript::OnCollisionEnter(Collider* other)
	{
		
	}
	void LastDoorScript::OnCollisionStay(Collider* other)
	{

		if (other->GetName() == L"Player")
		{
			if (other->GetOwner()->GetComponent<Animator>()->IsComplete())
				mbIsClear = true;
				
		}
	}
	void LastDoorScript::OnCollisionExit(Collider* other)
	{
	}
}