#include "MEScoreScene.h"
#include "MEGameObject.h"
#include "MEPlayer.h"
#include "METransform.h"
#include "MESpriteRenderer.h"


ME::ScoreScene::ScoreScene()
{
}

ME::ScoreScene::~ScoreScene()
{
}

void ME::ScoreScene::Initialize()
{
	Player* bg = new Player();

	Transform* tr = bg->AddComponent<Transform>();

	tr->SetPosition(Vector2(0, 0));
	tr->SetName(L"TR");

	SpriteRenderer* sr = bg->AddComponent<SpriteRenderer>();
	
	sr->SetName(L"SR");


	AddGameObject(bg, enums::eLayerType::BackGround);
}

void ME::ScoreScene::Update()
{
	Scene::Update();
}

void ME::ScoreScene::LateUpdate()
{
	Scene::LateUpdate();
}

void ME::ScoreScene::Render(HDC mHdc)
{
	Scene::Render(mHdc);
}
