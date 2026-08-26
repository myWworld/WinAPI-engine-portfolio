#include "MEGameOverScene.h"
#include "MEObject.h"
#include "MERenderer.h"
#include "MESpriteRenderer.h"
#include "METexture.h"
#include "MEResources.h"
#include "MEInput.h"

#include "MEApplication.h"

extern ME::Application application;


namespace ME
{
	GameOverScene::GameOverScene()
	{
	}
	GameOverScene::~GameOverScene()
	{
	}
	void GameOverScene::Initialize()
	{

		Vector2 resolution = Vector2(application.GetWidth(), application.GetHeight());

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, resolution / 2.0f);
		mCameraComp = camera->AddComponent<Camera>();
		renderer::mainCamera = mCameraComp;
		mCameraComp->SetName(L"MainCamera");

		GameObject* bg = object::Instantiate<GameObject>(enums::eLayerType::BackGround, Vector2(250, 60));
		
		SpriteRenderer* sr = bg->AddComponent<SpriteRenderer>();
		sr->SetSize(Vector2(0.3, 0.3));
		
		graphics::Texture* bgTexture = Resources::Find<graphics::Texture>(L"GAMEOVER");
		sr->SetTexture(bgTexture);

		Scene::Initialize();

	}
	void GameOverScene::Update()
	{
		Scene::Update();
	}
	void GameOverScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::Space))
		{
			SceneManager::LoadScene(L"TitleScene");


		}
	}
	void GameOverScene::Render(HDC mHdc)
	{
		Scene::Render(mHdc);
		wchar_t str[50] = L"처음으로 돌아가려면 SPACE를 누르시오.";
		UINT len = wcslen(str);

		TextOut(mHdc, 340, 400, str, len);
	}
	void GameOverScene::OnEnter()
	{
		renderer::mainCamera = mCameraComp;
		Scene::OnEnter();

	}
	void GameOverScene::OnExit()
	{
		Scene::OnExit();

	}
}