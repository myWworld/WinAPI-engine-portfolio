#pragma once
#include "../MyEngine_Source/MEGameObject.h"

namespace ME
{
	class Monsters :public GameObject
	{
	public:

		enum class eState
		{
			Alive,
			Dead,
		};

		Monsters();
		~Monsters();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		virtual float GetHp() { return mHp; };
		virtual void SetHp(float newHp) { mHp = newHp; }

		void SetMonsterState(eState state) { mState = state; }

		void GetPosForHp();

		bool IsMonsterDead()
		{
			if (mState == eState::Dead)return true;
			else return false;
		}

	private:
		
		float mHp;
		
		eState mState;

		float mPosForHp;

	};

}

