#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class StarScript :public Script
	{
	public:


		StarScript();
		~StarScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;




	private:
	};

}
