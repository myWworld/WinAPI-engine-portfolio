#include "MEUIHUD.h"
#include "MEResources.h"
#include "../MyEngine_W/MEPlayerScript.h"

namespace ME
{
	using namespace Gdiplus;

	ME::UIHUD::UIHUD()
		:UIBase(enums::eUIType::HpBar)
	{
	}

	ME::UIHUD::~UIHUD()
	{
	}

	void ME::UIHUD::OnInit()
	{
		SetPos(Vector2(5, 5));
		SetSize(Vector2(0.1f, 0.1f));

		
		mTexture = Resources::Find<graphics::Texture>(L"HPBAR");
	}

	void ME::UIHUD::OnActive()
	{
	}

	void ME::UIHUD::OnInActive()
	{
	}

	void ME::UIHUD::OnUpdate()
	{
	}

	void ME::UIHUD::OnLateUpdate()
	{
	}

	void ME::UIHUD::OnRender(HDC hdc)
	{

		if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Png)
		{
			Gdiplus::ImageAttributes imgAtt = {};

			imgAtt.SetColorKey(Gdiplus::Color(230, 230, 230), Gdiplus::Color(255, 255, 255));

			Gdiplus::Graphics graphics(hdc);

			if (mTexture->GetImage() == nullptr)
				return;

			graphics.DrawImage(mTexture->GetImage()
				, Gdiplus::Rect
				(
					mPosition.x
					, mPosition.y
					, mTexture->GetWidth() * mSize.x
					, mTexture->GetHeight() * mSize.y
				)
				, 0, 0
				, mTexture->GetWidth()
				, mTexture->GetHeight()
				, Gdiplus::UnitPixel
				, nullptr
			);
		}
		else if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Bmp)
		{
			TransparentBlt(hdc
				, mPosition.x, mPosition.y
				, mPosition.x + mSize.x
				, mPosition.y + mSize.y
				, mTexture->GedHdc()
				, 0, 0
				, mTexture->GetWidth()
				, mTexture->GetHeight()
				, RGB(255, 0, 255));
		}

		RECT rect = { mPosition.x + 27, mPosition.y + 5, mPosition.x + (85 * (PlayerScript::GetHp())/100), mPosition.y + 19};

		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, brush);
	}

	void ME::UIHUD::OnClear()
	{
	}

}