#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class AxeScript :public Script
	{
	public:


		AxeScript();
		~AxeScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;


		static bool IsTrigger() { return mbIsTrigger; }

	private:

		

	private:

		static bool mbIsTrigger;
	};

}
