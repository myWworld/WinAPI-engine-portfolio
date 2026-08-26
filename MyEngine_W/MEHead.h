#pragma once
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/MEAnimator.h"

namespace ME
{
	class Head :public GameObject
	{
	public:


		Head();
		~Head();

		void Initialize()override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;


	private:

	};


}
