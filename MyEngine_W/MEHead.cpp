#include "MEHead.h"


namespace ME
{
	Head::Head()
	{
	}
	Head::~Head()
	{
	}
	void Head::Initialize()
	{
		GameObject::Initialize();
	}
	void Head::Update()
	{
		GameObject::Update();
	}
	void Head::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Head::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}