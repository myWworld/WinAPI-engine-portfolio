#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class FlagScript :public Script
	{
	public:


		FlagScript();
		~FlagScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;


		static bool IsOnFlag() { return mbIsOnFlag; }
		static void SetFlag(bool isFlag) { mbIsOnFlag = isFlag; }

	private:

		static bool mbIsOnFlag;
	};

}
