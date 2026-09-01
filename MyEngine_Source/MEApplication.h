#pragma once

#include "CommonInclude.h"
#include "MEGameObject.h"
#include "MEScenes.h"



namespace ME
{
	

	class Application
	{

	public:

		Application();
		~Application();

		bool Initialize(HWND hwnd, UINT width, UINT height);

		void Run();
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();

		void Release();


		HDC GetHdc() { return mHdc; }
		HWND GetHwnd() { return mHwnd; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

		
	private:
		
		void ClearRenderTarget();
		void CopyRenderTarget(HDC source, HDC dest);
		void adjustWindowRect(HWND hwnd, UINT width, UINT height);
		bool createBuffer(UINT width, UINT height);
		void initializeEtc();

		void releaseBuffer() noexcept;
		void releaseWindowDC() noexcept;

	private:

		HWND mHwnd;
		HDC mHdc;

		// CreateCompatibleDC로 생성한 Memory DC
		HDC mBackHdc;

		// 생성한 실제 Back Buffer
		HBITMAP mBackBuffer;

		// mBackBuffer를 선택하기 전 Memory DC에 들어 있던 Bitmap
		// 삭제하지 않고, 종료 시 다시 선택
		HGDIOBJ mOldBackBitmap;


		UINT mWidth;
		UINT mHeight;

		float mSpeed;

		

		std::vector <Scene*> mScenes;
	};

}

