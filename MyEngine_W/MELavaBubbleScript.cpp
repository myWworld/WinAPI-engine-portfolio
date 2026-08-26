#include "MELavaBubbleScript.h"
#include "MEAnimator.h"
#include "MEGameObject.h"
#include "MEBoxCollider2D.h"
#include "METime.h"
#include "MERigidbody.h"
#include "METransform.h"
#include "MEInput.h"

namespace ME
{
	LavaBubbleScript::LavaBubbleScript()
		:mLauchTime(0.0f)
		, mbIsLaunched(false)
		,mStartTime(0.0f)
		,mLauchHeight(160.0f)
		
	{
	}
	LavaBubbleScript::~LavaBubbleScript()
	{
	}
	void LavaBubbleScript::Initialize()
	{
	}
	void LavaBubbleScript::Update()
	{
		mLauchTime += Time::DeltaTime();
		mStartTime += Time::DeltaTime();
	
		Animator* animator = GetOwner()->GetComponent<Animator>();

		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 velocity = rb->GetVelocity();
		
		BoxCollider2D* col = GetOwner()->GetComponent<BoxCollider2D>();

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
	
		if (mStartStandard < mStartTime)
		{
			if (mLauchTime > 2.0f && mbIsLaunched == false)
			{
				mbIsLaunched = true;

				tr->SetPosition(mLauchPos);

				velocity.y -= mLauchHeight;

				rb->SetNeedGravity(true, Vector2(0.0f, 170.0f));

				animator->PlayAnimation(L"UpR");
				GetOwner()->SetNoRender(true);



				rb->SetVelocity(velocity);
			}

			if (velocity.y >= 0 && mbIsLaunched == true)
			{
				animator->PlayAnimation(L"DownR");
			}

			float topSideOfLavaBubble = pos.y + col->GetOffset().y;
			float bottomSideOfLavaBubble = topSideOfLavaBubble + col->GetSize().y;


			if ((mLauchPos.y + col->GetOffset().y <= bottomSideOfLavaBubble) && mbIsLaunched == true)
			{
				mLauchTime = 0.0f;
				rb->SetNeedGravity(false);

				mbIsLaunched = false;
				GetOwner()->SetNoRender(false);
			}
		}

		if (mStartTime > 1e9)
		{
			mStartTime = mStartStandard;
		}
	


	}
	void LavaBubbleScript::LateUpdate()
	{
	}
	void LavaBubbleScript::Render(HDC hdc)
	{
	}
	void LavaBubbleScript::OnCollisionEnter(Collider* other)
	{
		if (other->GetName() == L"Player")
		{

		}
	}

	void LavaBubbleScript::OnCollisionStay(Collider* other)
	{
		
	}

	void LavaBubbleScript::OnCollisionExit(Collider* other)
	{

	}

}