#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class HorizonMovingBlockScript :public Script
	{
	public:

		enum class Direction
		{
			Left,
			Right
		};

		HorizonMovingBlockScript();
		~HorizonMovingBlockScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetLength(float maxLength, float minLength) { mMaxLength = maxLength; mMinLength = minLength; }

	private:

		void CannotPass(Collider* obj);

		void MovingBlockByItsDirection();
		void MovingPlayerByIsOnBlock();
		

	private:

		float mMaxLength;
		float mMinLength;

		bool mbIsOnBlock;

		Direction mDirection;

		GameObject* mObjectIsOnBlock;

	};

}