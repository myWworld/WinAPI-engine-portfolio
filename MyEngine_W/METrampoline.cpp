#include "METrampoline.h"

namespace ME
{
	Trampoline::Trampoline()
	{
	}
	Trampoline::~Trampoline()
	{
	}
	void Trampoline::Initialize()
	{
		GameObject::Initialize();
	}
	void Trampoline::Update()
	{
		GameObject::Update();
	}
	void Trampoline::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Trampoline::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}