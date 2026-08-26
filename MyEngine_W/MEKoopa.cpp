#include "MEKoopa.h"


namespace ME
{
	Koopa::Koopa()
	{
	}
	Koopa::~Koopa()
	{
	}
	void Koopa::Initialize()
	{
		Monsters::Initialize();
	}
	void Koopa::Update()
	{
		Monsters::Update();
	}
	void Koopa::LateUpdate()
	{
		Monsters::LateUpdate();
	}
	void Koopa::Render(HDC hdc)
	{
		Monsters::Render(hdc);
	}
}