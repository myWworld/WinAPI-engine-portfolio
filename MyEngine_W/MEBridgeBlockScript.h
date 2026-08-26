#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class BridgeBlockScript :public Script
	{
	public:


		BridgeBlockScript();
		~BridgeBlockScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetVanishInterval(float interval) { mVanishgInterval = interval; }


	private:

		void CannotPass(Collider* obj);

	private:

		float mVanishgInterval;
		float mVanishgTime;

		bool mbIsTriggered;
		bool mbIsOnFloor;
	};

}
