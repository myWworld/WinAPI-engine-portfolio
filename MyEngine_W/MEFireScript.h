#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "MEBullet.h"
#include "CommonInclude.h"
#include "MEPlayerScript.h"
#include "../MyEngine_Source/MECollider.h"

namespace ME
{


	class FireScript :public Script
	{
	public:

		FireScript();
		~FireScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;
		void Destroy();

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;



	private:

		void MoveFireByDirection();

	private:

		float mTime;

	};

}
