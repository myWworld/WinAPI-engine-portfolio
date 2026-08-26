#include "MEFireBarScript.h"
#include "MEResource.h"
#include "MEScenes.h"
#include "MEInput.h"
#include "MEGameOverScene.h"
#include "MESceneManager.h"
#include "METransform.h"
#include "CommonInclude.h"
#include "MECollider.h"
#include "MEBoxCollider2D.h"
#include "MERigidbody.h"
#include "METime.h"

namespace ME
{
	FireBarScript::FireBarScript()
		:mRotateTime(0.0f)
		, mStartTime(0.0f)
	{
	}
	FireBarScript::~FireBarScript()
	{
	}
	void FireBarScript::Initialize()
	{
	}
	void FireBarScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		BoxCollider2D* col = GetOwner()->GetComponent<BoxCollider2D>();
		
		mRotateTime += Time::DeltaTime();
		mStartTime += Time::DeltaTime();

		if (mStartTime > mRotateStart)
		{
			if (mRotateTime > 0.107f)
			{
				mCurRot -= mRot;
				tr->SetRotation(mCurRot);
				col->SetRotation(mCurRot);

				mRotateTime = 0;

				if (mCurRot <= -360)
				{
					mCurRot = 0;
				}
			}
		}

		if (mStartTime > 1e9)
		{
			mStartTime = mRotateStart;
		}

	}
	void FireBarScript::LateUpdate()
	{
	}
	void FireBarScript::Render(HDC hdc)
	{
	}
	void FireBarScript::OnCollisionEnter(Collider* other)
	{
	}
	void FireBarScript::OnCollisionStay(Collider* other)
	{
	}
	void FireBarScript::OnCollisionExit(Collider* other)
	{
	}
}