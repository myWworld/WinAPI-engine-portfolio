#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "MEBullet.h"
#include "CommonInclude.h"
#include "MEPlayerScript.h"
#include "../MyEngine_Source/MECollider.h"

namespace ME
{


	class BulletScript:public Script
	{
	public:

		BulletScript();
		~BulletScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;
		void Destroy();

		void OnCollisionEnter( Collider* other) override;
		void OnCollisionStay( Collider* other) override;
		void OnCollisionExit( Collider* other) override;

		void SetPlayer(GameObject* gameObj) { mPlayer = gameObj; }
		
		void SetPlayerIsRunningAttack(bool run)
		{
			if (run == true)
				mbIsPlayerRunning = true;
			else
				mbIsPlayerRunning = false;
		}

	private:

		void MoveBulletByDirectionAndPlayerRunning();

	private:

		bool mbIsPlayerRunning;

		GameObject* mPlayer;
	};

}
