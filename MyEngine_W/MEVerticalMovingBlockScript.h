#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class VerticalMovingBlockScript :public Script
	{
	public:

		enum class Direction
		{
			Up,
			Down
		};

		VerticalMovingBlockScript();
		~VerticalMovingBlockScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetHeight(float maxHeight, float minHeight) { mMaxHeight = maxHeight; mMinHeight = minHeight; }

	private:

		void CannotPass(Collider* obj);

	private:

		float mMaxHeight;
		float mMinHeight;

		bool mbIsOnBlock;

		Direction mDirection;

	};

}