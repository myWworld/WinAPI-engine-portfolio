#include "MEQboxScript.h"

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

#include "CommonInclude.h"

#include "MERenderer.h"
namespace ME
{



	ME::QboxScript::QboxScript()
		:mbIsBroken(false)
		, mbIsOnQbox(false)
		,mbTouchQboxBottom(false)
	{
	}

	ME::QboxScript::~QboxScript()
	{
	}

	void ME::QboxScript::Initialize()
	{
	}

	void ME::QboxScript::Update()
	{
	}

	void ME::QboxScript::LateUpdate()
	{
	}

	void ME::QboxScript::Render(HDC hdc)
	{
	}

	void ME::QboxScript::OnCollisionEnter(Collider* other)
	{

		if (other->GetName() == L"Cannon"
			|| other->GetName() == L"Bullet")
			return;

		CannotPass(other);

		if (other->GetName() == L"Player")
		{
			if (mbIsBroken == false && mbTouchQboxBottom == true)
			{
				mbIsBroken = true;
				GetOwner()->GetComponent<Animator>()->PlayAnimation(L"UsedR", true);

				int prob = rand() % 8;

				if (prob < 3)
				{
					CreateStar(other);
				}
				else
				{
					CreateCoin(other);
				}

			}
		}

	


	}
	void ME::QboxScript::OnCollisionStay(Collider* other)
	{
		if (other->GetName() == L"Cannon")
			return;

		if (other->GetName() == L"Player")
		{
			if (Input::GetKey(eKeyCode::Space))
				return;

		}

			CannotPass(other);
	}

	void ME::QboxScript::OnCollisionExit(Collider* other)
	{

		Rigidbody* rb = other->GetOwner()->GetComponent<Rigidbody>();
		rb->SetGround(false);
		mbIsOnQbox = false;
		mbTouchQboxBottom == false;
	}


	void QboxScript::CreateCoin(Collider* other)
	{
		GameObject* coin = object::Instantiate<GameObject>(enums::eLayerType::Items);

		BoxCollider2D* coinCol = coin->AddComponent<BoxCollider2D>();

		coin->AddComponent<CoinScript>();
		Rigidbody* coinRb = coin->AddComponent<Rigidbody>();

		Vector2 coinVel = coinRb->GetVelocity();
		coinVel.y -= 130.0f;
		coinRb->SetGround(false);
		coinRb->SetVelocity(coinVel);


		coinCol->SetName(L"Coin");
		coinCol->SetOffset(Vector2(-58, -60));
		coinCol->SetSize(Vector2(0.12f, 0.14f));

		Animator* coinAnimator = coin->AddComponent<Animator>();
		graphics::Texture* coinTex = Resources::Find<graphics::Texture>(L"COIN");

		coinAnimator->CreateAnimation(L"IdleR", coinTex, Vector2(0, 15), Vector2(120, 120), Vector2::Zero
			, 0.2f, 7, 6);

		coinAnimator->PlayAnimation(L"IdleR", true);

		coin->GetComponent<Transform>()->SetScale(Vector2(0.12f, 0.12f));

		Transform* qboxTr = GetOwner()->GetComponent<Transform>();
		Vector2 qboxPos = qboxTr-> GetPosition();

		coin->GetComponent<Transform>()->SetPosition(Vector2(qboxPos.x - 40, qboxPos.y - 60));
	}

	void QboxScript::CreateStar(Collider* other)
	{
		GameObject* star = object::Instantiate<GameObject>(enums::eLayerType::Items);
		BoxCollider2D* starCol = star->AddComponent<BoxCollider2D>();

		star->AddComponent<CoinScript>();
		Rigidbody* starRb = star->AddComponent<Rigidbody>();

		Vector2 starVel = starRb->GetVelocity();
		starVel.y -= 130.0f;
		starRb->SetGround(false);
		starRb->SetVelocity(starVel);

		starCol->SetName(L"Star");
		starCol->SetOffset(Vector2(-115, -115));
		starCol->SetSize(Vector2(0.16f, 0.16f));

		Animator* starAnimator = star->AddComponent<Animator>();

		star->GetComponent<Transform>()->SetScale(Vector2(0.08f, 0.08f));

		Vector2 qboxPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		star->GetComponent<Transform>()->SetPosition(Vector2(qboxPos.x + 16, qboxPos.y  ));

		graphics::Texture* starTex = Resources::Find<graphics::Texture>(L"STAR");

		starAnimator->CreateAnimation(L"IdleR", starTex, Vector2(0, 0), Vector2(236, 236), Vector2::Zero
			, 0.2f, 13, 12);

		starAnimator->PlayAnimation(L"IdleR", true);
	}

	void QboxScript::CannotPass(Collider* obj)
	{
		Rigidbody* playerRb = obj->GetOwner()->GetComponent<Rigidbody>();
		Transform* playerTr = obj->GetOwner()->GetComponent<Transform>();
		Collider* playerCol = obj;

		Transform* qboxTr = this->GetOwner()->GetComponent<Transform>();
		Vector2 qboxPos = qboxTr->GetPosition();

		Rigidbody* qboxRb = this->GetOwner()->GetComponent<Rigidbody>();
		Collider* qboxCol = this->GetOwner()->GetComponent<Collider>();


		Vector2 playerColSize = playerCol->GetSize() * 100.0f;
		Vector2 qboxColSize = qboxCol->GetSize() * 100.0f;

		Vector2 playerCenterPos = (playerTr->GetPosition() + playerCol->GetOffset()) + playerColSize / 2.0f;
		Vector2 qboxCenterPos = (qboxTr->GetPosition() + qboxCol->GetOffset()) + qboxColSize / 2.0f;

		float playerBottom = playerCenterPos.y + (playerColSize.y / 2.0f);
		float qboxTop = qboxCenterPos.y - (qboxColSize.y / 2.0f);

		float playerTop = playerCenterPos.y - (playerColSize.y / 2.0f);
		float qboxBottom = qboxCenterPos.y + (qboxColSize.y / 2.0f);

		Vector2 playerPos = playerTr->GetPosition();
	
		Vector2 qboxLeftTop = qboxTr->GetPosition() + qboxCol->GetOffset();

		if (playerCenterPos.x <= qboxLeftTop.x && playerCenterPos.x >= qboxLeftTop.x + qboxColSize.x)
		{
			float direct = playerCenterPos.x - qboxCenterPos.x;
	
			if (direct <= 0)
			{
				playerPos.x -= 1.5f;
			}
			else
			{
				playerPos.x += 1.5f;

			}
			playerTr->SetPosition(playerPos);
		}
		else
		{
			if (fabs(playerBottom - qboxTop) <= 4)
			{
				float len = fabs(playerCenterPos.y - qboxCenterPos.y);
				float scale = fabs(playerColSize.y / 2.0f + qboxColSize.y / 2.0f);

				if (len < scale)
				{
					Vector2 playerPos = playerTr->GetPosition();
					playerPos.y -= (scale - len) - 1.0f;
					playerTr->SetPosition(playerPos);
					mbIsOnQbox = true;
				}


				playerRb->SetGround(true);

			}

			if (fabs(playerTop - qboxBottom) <= 4)
			{
				

				float len = fabs(playerCenterPos.y - qboxCenterPos.y);
				float scale = fabs(playerColSize.y / 2.0f + qboxColSize.y / 2.0f);

				if (len < scale)
				{
					Vector2 playerPos = playerTr->GetPosition();
					playerPos.y += (scale - len) + 1.0f;
					playerTr->SetPosition(playerPos);

					Vector2 velocity = playerRb->GetVelocity();
					velocity.y += 30;
					playerRb->SetVelocity(velocity);
					
					
					mbTouchQboxBottom = true;
				}

			}
		}





	}
}