#include "MEBullet.h"

namespace ME
{
	Bullet::Bullet()
		:mTime(0.0f)
		,mbUsing(false)
	{
	}
	Bullet::~Bullet()
	{
	}
	void Bullet::Initialize()
	{
		GameObject::Initialize();
	}
	void Bullet::Update()
	{
		GameObject::Update();
	}
	void Bullet::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Bullet::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}