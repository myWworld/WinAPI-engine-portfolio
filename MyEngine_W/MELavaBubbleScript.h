#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class LavaBubbleScript :public Script
	{
	public:


		LavaBubbleScript();
		~LavaBubbleScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetLauchPos(Vector2 pos) { mLauchPos = pos; }
		void SetLauchStartTime(float start) { mStartStandard = start; }

		void SetLauchHeight(float height) { mLauchHeight = height; }

	private:

	

	private:

		float mStartStandard;
		float mStartTime;

		float mLauchTime;

		float mLauchHeight;

		Vector2 mLauchPos;

		bool mbIsLaunched;
	};

}
