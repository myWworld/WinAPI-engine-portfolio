#pragma once
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/MEAnimator.h"

namespace ME
{
	class Bullet:public GameObject
	{
	public:

		enum class eDirection
		{
			Right,
			Left,
			Up,
			Down,
		};

		Bullet();
		~Bullet();

		void Initialize()override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		float GetBullletLastTime() { return mTime; }
		void SetBulletTime(float time) { mTime = time; }

		void SetDirection(eDirection direction) { mDirection = direction; }
		eDirection GetDirection() { return mDirection; }


	private:

		float mTime;

		eDirection mDirection;

		bool mbUsing;

	};


}