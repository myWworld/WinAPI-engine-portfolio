#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class LastDoorScript :public Script
	{
	public:


		LastDoorScript();
		~LastDoorScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		static bool IsClearStage() { return mbIsClear; }
		static void SetClearStage(bool isClear) { mbIsClear = isClear; }


	private:

		static bool mbIsClear;
	};

}