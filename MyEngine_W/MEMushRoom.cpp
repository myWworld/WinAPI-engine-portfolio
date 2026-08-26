#include "MEMushRoom.h"
#include "METransform.h"


namespace ME
{
	MushRoom::MushRoom()
	{
	}
	MushRoom::~MushRoom()
	{
	}
	void MushRoom::Initialize()
	{
		Monsters::Initialize();
	}
	void MushRoom::Update()
	{
		Monsters::Update();
	}
	void MushRoom::LateUpdate()
	{
		Monsters::LateUpdate();
	}
	void MushRoom::Render(HDC hdc)
	{
		Monsters::Render(hdc);
	}

}