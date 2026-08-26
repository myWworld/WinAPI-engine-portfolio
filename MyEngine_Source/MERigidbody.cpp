#include "MERigidbody.h"
#include "METime.h"
#include "METransform.h"
#include "MEGameObject.h"
namespace ME
{
	Rigidbody::Rigidbody()
		:Component(enums::eComponentType::Rigidbody)
		, mbGround(false)
		, mMass(1.0f)
		, mFriction(70.0f)
	, mForce(Vector2::Zero)
	, mVelocity(Vector2::Zero)
	, mLimitVelocity(Vector2(130.0f,800.0f))
	, mGravity(Vector2(0.0f, 800.0f))
	, mAccelation(Vector2::Zero)
		, mbIsAffectedByGravity(true)
	{


	}
	Rigidbody::~Rigidbody()
	{
	}
	void Rigidbody::Initialize()
	{
	}
	void Rigidbody::Update()
	{
		// 1. 뉴턴의 운동 법칙 (F = ma) 적용
		mAccelation = mForce / mMass;

		// 2. 가속도와 중력을 속도에 누적 (v = v0 + at)
		mVelocity += mAccelation * Time::DeltaTime();

		if (mbGround) // 땅에 있을 때 중력 상쇄 연산
		{
			Vector2 gravity = mGravity;
			gravity.normalize();
			float dot = math::Vector2::Dot(mVelocity, gravity);
			mVelocity -= gravity * dot;
		}
		else // 공중에 있을 때 중력 가속도 적용
		{
			mVelocity += mGravity * Time::DeltaTime();
		}

		// 3. 최대 속도 제한 (Terminal Velocity 클램핑)
		Vector2 gravity = mGravity;
		gravity.normalize();
		float dot = math::Vector2::Dot(mVelocity, gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;

		if (mLimitVelocity.y < gravity.length()) {
			gravity.normalize();
			gravity *= mLimitVelocity.y;
		}
		if (mLimitVelocity.x < sideVelocity.length()) {
			sideVelocity.normalize();
			sideVelocity *= mLimitVelocity.x;
		}
		mVelocity = gravity + sideVelocity;

		// 4. 마찰력(Friction) 적용 및 최종 위치 계산
		if (!(mVelocity == Vector2::Zero))
		{
			// 이동 방향과 반대되는 마찰력 벡터 계산
			Vector2 friction = -mVelocity;
			friction = friction.normalize() * mFriction * mMass * Time::DeltaTime();

			// 마찰력으로 인한 속도 감소량이 현재 속도를 초과하면 완전히 정지 (Jittering 방지)
			if (mVelocity.length() <= friction.length()) {
				mVelocity = Vector2::Zero;
			}
			else {
				mVelocity += friction;
			}

			// 최종 속도를 바탕으로 Transform 위치 갱신
			Transform* tr = GetOwner()->GetComponent<Transform>();
			Vector2 pos = tr->GetPosition();
			pos = pos + mVelocity * Time::DeltaTime();
			tr->SetPosition(pos);

			mForce.clear(); // 프레임 종료 시 작용한 힘 초기화
		}
		

	}
	void Rigidbody::LateUpdate()
	{
	}
	void Rigidbody::Render(HDC hdc)
	{
	}

	void Rigidbody::StopMoving()
	{
		mForce.clear();
		mVelocity = Vector2::Zero;
	}
}