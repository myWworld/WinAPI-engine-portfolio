#include "METurtle.h"

namespace ME
{


	ME::Turtle::Turtle()
		:mHp(100.0f)
	{
	}

	ME::Turtle::~Turtle()
	{
	}

	void ME::Turtle::Initialize()
	{

		Monsters::Initialize();
	}

	void ME::Turtle::Update()
	{

		Monsters::Update();
	}

	void ME::Turtle::LateUpdate()
	{
		Monsters::LateUpdate();
	}

	void ME::Turtle::Render(HDC hdc)
	{
		Monsters::Render(hdc);
	}

}