#include "MEBlock.h"


namespace ME
{
	Block::Block()
	{
	}
	Block::~Block()
	{
	}
	void Block::Initialize()
	{
		GameObject::Initialize();
	}
	void Block::Update()
	{
		GameObject::Update();
	}
	void Block::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Block::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}