#include "MECoinScript.h"
#include "MEInput.h"
#include "METransform.h"
#include "METime.h"
#include "MEGameObject.h"
#include "MEBulletScript.h"
#include "MEBullet.h"
#include "MEObject.h"
#include "MEResources.h"
#include "MEBoxCollider2D.h"
#include "MESceneManager.h"
#include "MEScenes.h"
#include "MERigidbody.h"
#include "MECoinScript.h"
#include "MEPlayerScript.h"
#include "CommonInclude.h"

namespace ME
{
	CoinScript::CoinScript()
	{
	}
	CoinScript::~CoinScript()
	{
	}
	void CoinScript::Initialize()
	{
	}
	void CoinScript::Update()
	{
	}
	void CoinScript::LateUpdate()
	{
	}
	void CoinScript::Render(HDC hdc)
	{
	}
	void CoinScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{
			GameObject* coin = GetOwner();

			coin->SetActive(false);
			coin->SetDeath();

			PlayerScript::PlusScore();

		}
	}
	void CoinScript::OnCollisionStay(Collider* other)
	{
	}
	void CoinScript::OnCollisionExit(Collider* other)
	{
	}
}