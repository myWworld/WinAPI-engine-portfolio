#pragma once
#include "../MyEngine_Source/MEScript.h"

#include "../MyEngine_Source/MEGameObject.h"

namespace ME
{


	class PulleyBlockScript :public Script
	{
	public:

		enum class Direction
		{
			Up,
			Down
		};

		PulleyBlockScript();
		~PulleyBlockScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetHeight(float maxHeight, float minHeight) { mMaxHeight = maxHeight; mMinHeight = minHeight; }
		void SetOtherSide(GameObject* otherSide) { mOtherBlock = otherSide; }

	


	private:

		void CannotPass(Collider* obj);

	private:
		
		GameObject* mOtherBlock;

		float mMaxHeight;
		float mMinHeight;

		bool mbIsOnBlock;

		Direction mDirection;

	};

}