#include "MEFlag.h"
namespace ME
{
	Flag::Flag()
	{
	}
	Flag::~Flag()
	{
	}
	void Flag::Initialize()
	{
		GameObject::Initialize();
	}
	void Flag::Update()
	{
		GameObject::Update();
	}
	void Flag::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Flag::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}