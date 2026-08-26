#pragma once
#include "../MyEngine_Source/MEGameObject.h"
#include "MEMonsters.h"

namespace ME
{
	class Koopa :public Monsters
	{
	public:

		Koopa();
		~Koopa();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;


		float GetHp()override { return mHp; }
		void SetHp(float newHp) override { mHp = newHp; }

	private:
		float mHp;
	};

}

