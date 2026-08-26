#include "MEFlagScript.h"
#include "METransform.h"
#include "MEResources.h"
#include "MESceneManager.h"
#include "MEInput.h"
#include "METime.h"


namespace ME
{
	bool FlagScript::mbIsOnFlag = false;

	FlagScript::FlagScript()
		
	{
	}
	FlagScript::~FlagScript()
	{
	}
	void FlagScript::Initialize()
	{
	}
	void FlagScript::Update()
	{

	}
	void FlagScript::LateUpdate()
	{
	}
	void FlagScript::Render(HDC hdc)
	{
	}
	void FlagScript::OnCollisionEnter(Collider* other)
	{


		if (other->GetName() == L"Player")
		{
			mbIsOnFlag = true;
		}
	}
	void FlagScript::OnCollisionStay(Collider* other)
	{
	}
	void FlagScript::OnCollisionExit(Collider* other)
	{
	}
}