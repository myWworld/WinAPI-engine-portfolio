#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class TrampolineScript :public Script
	{
	public:


		TrampolineScript();
		~TrampolineScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetoriginOffsetY(float offsetY) { originOffsetY = offsetY; }


	private:

		void CannotPass(Collider* obj);

	private:

		bool mbIsOnTrampoline;

		float originOffsetY;
		float minHeight;
	};

}