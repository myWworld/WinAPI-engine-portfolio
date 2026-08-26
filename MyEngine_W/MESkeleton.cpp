#include "MESkeleton.h"

namespace ME
{
	void Skeleton::Initialize()
	{
		Monsters::Initialize();
	}
	void Skeleton::Update()
	{
		Monsters::Update();
	}
	void Skeleton::LateUpdate()
	{
		Monsters::LateUpdate();
	}
	void Skeleton::Render(HDC hdc)
	{
		Monsters::Render(hdc);
	}
}