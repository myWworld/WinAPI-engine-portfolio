#include "MEMonsters.h"
#include "METransform.h"
#include "MECollider.h"


	namespace ME
	{
		Monsters::Monsters()
			:mState(eState::Alive)
			,mHp(100.0f)
		{
		}
		Monsters::~Monsters()
		{
		}
		void Monsters::Initialize()
		{
			GameObject::Initialize();
		}
		void Monsters::Update()
		{
			GameObject::Update();
		}
		void Monsters::LateUpdate()
		{
			GameObject::LateUpdate();
		}
		void Monsters::Render(HDC hdc)
		{
			GameObject::Render(hdc);
		}

		void Monsters::GetPosForHp()
		{
			
		}
	}
