#include "MEApplication.h"
#include "MEInput.h"
#include "METime.h"
#include "MESceneManager.h"
#include "MEResources.h"
#include "MECollisionManager.h"
#include "MEUIManager.h"

#include "MEFmod.h"


namespace ME 
{

	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
		, mBackHdc(nullptr)
		, mBackBuffer(nullptr)
		, mOldBackBitmap(nullptr)
		, mWidth(0)
		, mHeight(0)
		, mSpeed(0.0f)
	{
	}

	
	Application::~Application()
	{
		// Release()를 깜빡하더라도
		// Application이 직접 소유한 GDI 자원은 정리
		//
		// Release()에서 이미 정리됐다면
		// 포인터가 nullptr이므로 중복 정리되지 않음
		releaseBuffer();
		releaseWindowDC();
	}

	bool Application::Initialize(HWND hwnd, UINT width, UINT height)
	{
		if (hwnd == nullptr || width == 0 || height == 0)
		{
			return false;
		}

		mHwnd = hwnd;
		mWidth = width;
		mHeight = height;

		adjustWindowRect(hwnd, width, height);

		// Window DC 획득
		mHdc = GetDC(mHwnd);

		if (mHdc == nullptr)
		{
			mHwnd = nullptr;
			return false;
		}

		if (!createBuffer(width, height))
		{
			releaseWindowDC();
			mHwnd = nullptr;
			return false;
		}

		initializeEtc();

		Fmod::Initialize();
		CollisionManager::Iniatialize();
		UIManager::Initailize();
		SceneManager::Initialize();

		return true;
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();

		Destroy();
	}

	void Application::Update()
	{
		Input::Update();
		Time::Update();

   		CollisionManager::Update();
		UIManager::Update();
		SceneManager::Update();

		
	}

	void Application::LateUpdate()
	{	
		CollisionManager::LateUpdate();
		UIManager::LateUpdate();
		SceneManager::LateUpdate();
	}

	void Application::Render()
	{
	
		ClearRenderTarget();

		Time::Render(mBackHdc);
		
		CollisionManager::Render(mBackHdc);
		SceneManager::Render(mBackHdc);
		UIManager::Render(mBackHdc);

		CopyRenderTarget(mBackHdc, mHdc);
		
	
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Release()
	{
		// 렌더링 자원을 사용하는 시스템을 먼저 종료
		SceneManager::Release();
		UIManager::Release();
		Resources::Release();

		// Application이 직접 소유한 GDI 자원 정리
		releaseBuffer();
		releaseWindowDC();

		mHwnd = nullptr;
		mWidth = 0;
		mHeight = 0;
	}

	void Application::ClearRenderTarget()
	{

		HBRUSH grayBrush = (HBRUSH)CreateSolidBrush(RGB(128,128,128));
		HBRUSH oldBrush = (HBRUSH)SelectObject(mBackHdc, grayBrush);

		Rectangle(mBackHdc, -1, -1, 1601, 901);

		SelectObject(mBackHdc, oldBrush);
		DeleteObject(grayBrush);

	}

	void Application::CopyRenderTarget(HDC source, HDC dest)
	{
		//백버퍼에 있는 것을 원본 버퍼에 복사
		BitBlt(dest, 0, 0, mWidth, mHeight,
			source, 0, 0, SRCCOPY);

	}

	void Application::adjustWindowRect(HWND hwnd, UINT width, UINT height)
	{
		mHwnd = hwnd;
		mHdc = GetDC(hwnd);


		RECT rect = { 0,0,width,height };

		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);



		SetWindowPos(mHwnd, nullptr, 0, 0, mWidth,mHeight, 0);
		ShowWindow(mHwnd, true);

	}
	bool Application::createBuffer(UINT width, UINT height)
	{
		if (mHdc == nullptr || width == 0 || height == 0)
		{
			return false;
		}

		// 재생성되는 경우를 대비해 기존 백버퍼부터 정리
		releaseBuffer();

		// 화면 DC와 호환되는 Memory DC 생성
		mBackHdc = CreateCompatibleDC(mHdc);

		if (mBackHdc == nullptr)
		{
			return false;
		}

		// 반드시 mBackHdc가 아니라 화면 DC인 mHdc를 사용
		// 새 Memory DC에는 초기 상태에서 단색 Bitmap이 선택되어 있기 때문
		mBackBuffer = CreateCompatibleBitmap(mHdc, width, height);

		if (mBackBuffer == nullptr)
		{
			DeleteDC(mBackHdc);
			mBackHdc = nullptr;

			return false;
		}

		// Bitmap을 Memory DC에 선택
		//  이전에 선택돼 있던 원래 Bitmap을 보관
		mOldBackBitmap = SelectObject(mBackHdc, mBackBuffer);

		if (mOldBackBitmap == nullptr ||
			mOldBackBitmap == HGDI_ERROR)
		{
			// SelectObject 실패 시 mBackBuffer는 선택되지 않았으므로
			// 그대로 삭제 가능
			DeleteObject(mBackBuffer);
			mBackBuffer = nullptr;

			DeleteDC(mBackHdc);
			mBackHdc = nullptr;

			mOldBackBitmap = nullptr;

			return false;
		}

		return true;
	}

	void Application::releaseBuffer() noexcept
	{
		//  mBackBuffer를 삭제하기 전에
		//    Memory DC에서 선택 해제
		if (mBackHdc != nullptr &&
			mOldBackBitmap != nullptr &&
			mOldBackBitmap != HGDI_ERROR)
		{
			SelectObject(mBackHdc, mOldBackBitmap);
			mOldBackBitmap = nullptr;
		}

		// mBackBuffer는 DC에 선택되어 있지 않으므로 삭제 가능
		if (mBackBuffer != nullptr)
		{
			DeleteObject(mBackBuffer);
			mBackBuffer = nullptr;
		}

		// CreateCompatibleDC로 만든 DC는 DeleteDC
		if (mBackHdc != nullptr)
		{
			DeleteDC(mBackHdc);
			mBackHdc = nullptr;
		}
	}

	void Application::releaseWindowDC() noexcept
	{
		if (mHdc != nullptr && mHwnd != nullptr)
		{
			// GetDC로 획득했으므로 DeleteDC가 아니라 ReleaseDC
			ReleaseDC(mHwnd, mHdc);
			mHdc = nullptr;
		}
	}

	void Application::initializeEtc()
	{
		Input::Initialize();
		Time::Intialize();

	}

}
