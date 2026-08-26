#pragma once
#include "CommonInclude.h"
#include "../MyEngine_Source/MEGameObject.h"

namespace ME
{


	class Laser : public GameObject
	{
	public:
		Laser();
		~Laser();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:

	};

}


