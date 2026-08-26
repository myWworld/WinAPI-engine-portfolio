#include "MECamera.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MEApplication.h"
#include <algorithm>

extern ME::Application application;

namespace ME
{
	Camera::Camera()
		:Component(enums::eComponentType::Camera)
		, mDistance(Vector2::Zero)
		, mResolution(Vector2::Zero)
		, mLookPosition(Vector2::Zero)
		, mTarget(nullptr)
		, mapTotalWidth(2500.0f)
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		mResolution.x = application.GetWidth();
		mResolution.y = application.GetHeight();
	}

	void Camera::Update()
	{
		if (mTarget)
		{
			Transform* tr = mTarget->GetComponent<Transform>();
			mLookPosition = tr->GetPosition();
			mLookPosition.y = mResolution.y / 2.0f;// Y축은 화면 중앙으로 고정

			//맵 바운더리 이탈 방지(Clamping)
			float minX = mResolution.x / 2.0f;
			float maxX = mapTotalWidth - (mResolution.x / 2.0f);

			mLookPosition.x = std::clamp(mLookPosition.x, minX, maxX);

			if(mTarget->GetComponent<Transform>()->GetName() == L"Player")
					mDistance = mLookPosition - Vector2(mResolution.x/2.0f,100);
			else
				mDistance = mLookPosition - (mResolution / 2.0f);
			
		}
		else
		{
			Transform* cameraTr = GetOwner()->GetComponent<Transform>();
			mLookPosition = cameraTr->GetPosition();
			mDistance = mLookPosition - (mResolution / 2.0f);
		}

	
		
	}

	void Camera::LateUpdate()
	{

	}
	void Camera::Render(HDC hdc)
	{
	}
}