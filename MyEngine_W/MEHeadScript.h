#pragma once
#include "../MyEngine_Source/MEScript.h"

namespace ME
{


	class HeadScript :public Script
	{
	public:


		HeadScript();
		~HeadScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		bool IsHeadShot() { return mbIsHead; }
		void SetHeadShot(bool isHead) { mbIsHead = isHead; }
		void SetHeadOwner(GameObject* owner) { mHeadOwner = owner; }

	private:

		void CannotPass(Collider* obj);
		void MakePlayerBounce(Collider* player);
		
	private:

		GameObject* mHeadOwner;
		bool mbIsHead;
	};

}
