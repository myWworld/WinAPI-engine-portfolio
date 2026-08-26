#pragma once
#include "../MyEngine_Source/MEGameObject.h"
#include "MEMonsters.h"

namespace ME
{


	class Skeleton:public Monsters
	{
	public:


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
