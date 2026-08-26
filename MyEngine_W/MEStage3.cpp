#include "MEStage3.h"

#include "ContentAndScript.h"

#include "MEGameObject.h"

#include "MEInput.h"
#include "METitleScene.h"
#include "MEObject.h"
#include "MEResources.h"
#include "METime.h"
#include "MEFmod.h"
#include "MEAudioClip.h"
#include "MEAudioSource.h"
#include "MEAudioListener.h"

#include "MESceneManager.h"
#include "METransform.h"
#include "MESpriteRenderer.h"
#include "MEBoxCollider2D.h"
#include "MERigidbody.h"

#include "MERenderer.h"
#include "MEApplication.h"

#include "MEUIManager.h"
#include "MEUIHUD.h"
#include "MEUIButton.h"

#include "METileMapRenderer.h"

#include "MECollisionManager.h"


extern ME::Application application;

namespace ME
{


	ME::Stage3::Stage3()
		:mCannonLaunchTime(0.0f)
	{
	}

	ME::Stage3::~Stage3()
	{
	}

	void ME::Stage3::Initialize()
	{
		{
			Vector2 resolution = Vector2(application.GetWidth(), application.GetHeight());

			GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, resolution / 2.0f);
			mCameraComp = camera->AddComponent<Camera>();
			mCameraComp->SetMapTotalWidth(2560.0f);
			renderer::mainCamera = mCameraComp;

		}

		{
			GameObject* background = object::Instantiate<GameObject>(enums::eLayerType::BackGround, Vector2(0, 0));
			SpriteRenderer* backgroundSr = background->AddComponent<SpriteRenderer>();

			graphics::Texture* backgroundTex = Resources::Find<graphics::Texture>(L"STAGE3");

			backgroundSr->SetTexture(backgroundTex);

		}

		{
			MakeFloor();
			MakeCeiling();
			MakeFireBar();
			CreateBridgeBlocks();
			
			MakeLavaBubble();

		



		}
		
		{
			Koopa* koopa = object::Instantiate<Koopa>(enums::eLayerType::Monster,Vector2(2150,80)); //Vector2(1247, 160)
			CreateKoopa(koopa);
		}

		{
			Flag* flag = object::Instantiate<Flag>(enums::eLayerType::Block, Vector2(2414, 195));
			CreateFlag(flag, 0.2, 0.26, -10, -15);

			LastDoor* lastdoor = object::Instantiate<LastDoor>(enums::eLayerType::Block, Vector2(2500, 165));
			CreateLastDoor(lastdoor, 0.27, 0.4);
		}

		Scene::Initialize();
	}

	void ME::Stage3::Update()
	{
		if (mPlayer == nullptr)
		{
			mPlayer = object::Instantiate<Player>
				(enums::eLayerType::Player, Vector2(120, 100));

			playerInitialize();

			renderer::mainCamera = mCameraComp;
			mCameraComp->SetTarget(mPlayer);
		}

		Scene::Update();
	}

	void ME::Stage3::LateUpdate()
	{
		if (LastDoorScript::IsClearStage())
		{
			LastDoorScript::SetClearStage(false);
			SceneManager::LoadScene(L"TitleScene");
		}


		Scene::LateUpdate();
	}

	void ME::Stage3::Render(HDC mHdc)
	{
		Scene::Render(mHdc);
	}

	void ME::Stage3::OnEnter()
	{
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Particle, enums::eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Particle, enums::eLayerType::Player, true);


		CollisionManager::CollisionLayerCheck(enums::eLayerType::Monster, enums::eLayerType::Floor, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Monster, enums::eLayerType::Block, true);

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Block, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Floor, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Obstacle, true);

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Items, enums::eLayerType::Block, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Items, enums::eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Items, enums::eLayerType::Floor, true);

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Bullet, enums::eLayerType::Block, true);

	//	mPlayer = object::Instantiate<Player>
		//	(enums::eLayerType::Player, Vector2(130, 120));

		//playerInitialize();
		PlayerScript::ReSetScore(0.0f);
		mPlayTime = 0.0f;

		FlagScript::SetFlag(false);

		UIManager::Push(enums::eUIType::HpBar);
		Scene::OnEnter();
	}

	void ME::Stage3::OnExit()
	{
		UIManager::Pop(enums::eUIType::HpBar);
		Scene::OnExit();
	}

	void ME::Stage3::CreatePlayerAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture)
	{
		animator->CreateAnimation(L"RightWalkR", Rtexture, Vector2(0, 212.0f), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.1f, 8, 3);
		animator->CreateAnimation(L"LeftWalkL", Ltexture, Vector2(356, 212.0f), Vector2(50, 50), Vector2(0, 0), 0.1f, 8, 3);

		animator->CreateAnimation(L"RunR", Rtexture, Vector2(0, 212.0f), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.07f, 8, 3);
		animator->CreateAnimation(L"RunL", Ltexture, Vector2(356, 212), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.07f, 8, 3);

		animator->CreateAnimation(L"JumpR", Rtexture, Vector2(0, 412), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.1f, 7);
		animator->CreateAnimation(L"JumpL", Ltexture, Vector2(356, 412), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.1f, 7);

		animator->CreateAnimation(L"StandingR", Rtexture, Vector2(301.0f, 10.0f), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.3f, 2, 8);
		animator->CreateAnimation(L"StandingL", Ltexture, Vector2(55.0f, 10.0f), Vector2(50.0f, 50.0f), Vector2(0, 0), 0.3f, 2, 8);

		animator->CreateAnimation(L"StandAttackR", Rtexture, Vector2(0, 112), Vector2(50.0f, 50.0f), Vector2::Zero, 0.1f, 3);
		animator->CreateAnimation(L"RunningAttackR", Rtexture, Vector2(245, 262), Vector2(50.0f, 50.0f), Vector2::Zero, 0.1f, 3, 8);

		animator->CreateAnimation(L"StandAttackL", Ltexture, Vector2(357, 112), Vector2(49.0f, 50.0f), Vector2::Zero, 0.1f, 3);
		animator->CreateAnimation(L"RunningAttackL", Ltexture, Vector2(100, 262), Vector2(50.0f, 50.0f), Vector2::Zero, 0.1f, 3, 8);

		animator->CreateAnimation(L"HurtL", Ltexture, Vector2(54, 162), Vector2(50.0f, 50.0f), Vector2::Zero, 0.1f, 2);
		animator->CreateAnimation(L"HurtR", Rtexture, Vector2(251, 162), Vector2(50.0f, 50.0f), Vector2::Zero, 0.1f, 2);

		animator->CreateAnimation(L"ClearL", Rtexture, Vector2(152, 582), Vector2(48.0f, 50.0f), Vector2::Zero, 0.45f, 4);
	}

	void Stage3::CreateKoopa(GameObject* koopa)
	{
		KoopaScript* koopaScript = koopa->AddComponent<KoopaScript>();
		koopaScript->SetEnemey(mPlayer);

		Animator* animator = koopa->AddComponent<Animator>();
		Rigidbody* rb = koopa->AddComponent<Rigidbody>();
		BoxCollider2D* col = koopa->AddComponent<BoxCollider2D>();

		Transform* tr = koopa->GetComponent<Transform>();
		tr->SetScale(Vector2(1.2f, 1.2f));
		
		col->SetSize(Vector2(0.38, 0.37f));
		col->SetOffset(Vector2(-13, -13));

		graphics::Texture* tex = Resources::Find<graphics::Texture>(L"KOOPA");
		animator->CreateAnimation(L"WalkR", tex, Vector2(158, 209), Vector2(39, 35), Vector2::Zero, 0.1f, 2);
		animator->CreateAnimation(L"WalkL", tex, Vector2(118, 209), Vector2(39, 35), Vector2::Zero, 0.1f, 2);

		animator->CreateAnimation(L"AttackR", tex, Vector2(238, 209), Vector2(39, 35), Vector2::Zero, 0.4f, 2);
		animator->CreateAnimation(L"AttackL", tex, Vector2(39, 209), Vector2(39, 35), Vector2::Zero, 0.4f, 2);

		animator->CreateAnimation(L"IdleR", tex, Vector2(158, 209), Vector2(39, 35), Vector2(0,0), 0.3f, 1);
		animator->CreateAnimation(L"IdleL", tex, Vector2(117, 209), Vector2(39, 35), Vector2::Zero, 0.3f, 1);

		animator->PlayAnimation(L"IdleL");
	}



	void ME::Stage3::CreateQbox(GameObject* qbox)
	{
		BoxCollider2D* qboxCol = qbox->AddComponent<BoxCollider2D>();

		qbox->AddComponent<QboxScript>();


		qboxCol->SetOffset(Vector2(-100, -100));
		qboxCol->SetSize(Vector2(0.15f, 0.15f));

		Animator* qboxAnimator = qbox->AddComponent<Animator>();

		qbox->GetComponent<Transform>()->SetScale(Vector2(0.08f, 0.08f));

		graphics::Texture* qboxTex = Resources::Find<graphics::Texture>(L"QBOX");
		graphics::Texture* usedQboxTex = Resources::Find<graphics::Texture>(L"USEDQBOX");

		qboxAnimator->CreateAnimation(L"IdleR", qboxTex, Vector2(0, 0), Vector2(200, 200), Vector2::Zero
			, 0.2f, 4);
		qboxAnimator->CreateAnimation(L"UsedR", usedQboxTex, Vector2(0, 0), Vector2(240, 242), Vector2(17, 17)
			, 0.2f, 1);

		qboxAnimator->PlayAnimation(L"IdleR", true);
	}

	void Stage3::CreateFloor(GameObject* floor, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* floorCol = floor->AddComponent<BoxCollider2D>();
		floor->AddComponent<FloorScript>();
		floorCol->SetName(L"Floor");

		floorCol->SetSize(Vector2(xSize, ySize));
		floorCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage3::CreateCeiling(GameObject* ceiling, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* ceilingCol = ceiling->AddComponent<BoxCollider2D>();
		ceiling->AddComponent<CeilingScript>();
		ceilingCol->SetName(L"Ceiling");

		ceilingCol->SetSize(Vector2(xSize, ySize));
		ceilingCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage3::CreateLastDoor(GameObject* lastdoor, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* lastDoorCol = lastdoor->AddComponent<BoxCollider2D>();
		lastdoor->AddComponent<LastDoorScript>();
		lastDoorCol->SetName(L"LastDoor");

		lastDoorCol->SetSize(Vector2(xSize, ySize));
		lastDoorCol->SetOffset(Vector2(xOffset, yOffset));
	}
	
	void Stage3::CreateStair(Vector2 startPos, int maxSteps, int height, char direction)
	{

		int stairSize = 16;
		float stairSizeForCol = stairSize / 100.0f;

		if (direction == 'R')
		{
			for (int i = 0; i < height; i++)
			{
				Block* stair = object::Instantiate<Block>(enums::eLayerType::Block
					, Vector2(startPos.x + stairSize * i, startPos.y - stairSize * i));

				BoxCollider2D* stairCol = stair->AddComponent<BoxCollider2D>();
				stair->AddComponent<BlockScript>();
				stairCol->SetName(L"Stair");

				stairCol->SetSize(Vector2(stairSizeForCol * (maxSteps - i), stairSizeForCol));

			}
		}
		else if (direction == 'L')
		{
			for (int i = 0; i < height; i++)
			{
				Block* stair = object::Instantiate<Block>(enums::eLayerType::Block
					, Vector2(startPos.x, startPos.y - stairSize * i));

				BoxCollider2D* stairCol = stair->AddComponent<BoxCollider2D>();
				stair->AddComponent<BlockScript>();
				stairCol->SetName(L"Stair");

				stairCol->SetSize(Vector2(stairSizeForCol * (maxSteps - i), stairSizeForCol));

			}
		}



	}
	void Stage3::CreateFlag(GameObject* flag, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* flagCol = flag->AddComponent<BoxCollider2D>();
		flag->AddComponent<FlagScript>();
		
		flagCol->SetName(L"Flag");
		flagCol->SetSize(Vector2(xSize, ySize));
		flagCol->SetOffset(Vector2(xOffset, yOffset));
		
		graphics::Texture* tex = Resources::Find<graphics::Texture>(L"STAGE3MAPS");

		Animator* flagAnimator = flag->AddComponent<Animator>();

		flagAnimator->CreateAnimation(L"IdleR", tex, Vector2(0, 0), Vector2(20, 26), Vector2::Zero, 0.1f, 1);
		flagAnimator->PlayAnimation(L"IdleR");

	}
	void Stage3::CreateBlock(GameObject* block, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* BlockCol = block->AddComponent<BoxCollider2D>();
		block->AddComponent<BlockScript>();
		BlockCol->SetName(L"Block");


		BlockCol->SetSize(Vector2(xSize, ySize));
		BlockCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage3::CreateFireBar(GameObject* firebar, float xSize, float ySize, float xOffset, float yOffset
		, float startTime)
	{
		
		FireBarScript* firebarscript = firebar->AddComponent<FireBarScript>();
		firebarscript->SetRotation(12.f);
		firebarscript->SetRotateStart(startTime);

		Transform* firebarTr = firebar->GetComponent<Transform>();
		//firebarTr->SetScale(Vector2(0.48, 0.48));
		

		BoxCollider2D* firebarCol = firebar->AddComponent<BoxCollider2D>();
		firebarCol->SetSize(Vector2(xSize, ySize));
		firebarCol->SetOffset(Vector2(xOffset, yOffset));

		Vector2 pos = firebarTr->GetPosition();
		Vector2 colOffset = firebarCol->GetOffset();

		Animator* fireBarAnimator = firebar->AddComponent<Animator>();

		graphics::Texture* fireBarTex = Resources::Find<graphics::Texture>(L"SHORTFIREBAR");
		fireBarAnimator->CreateAnimation(L"IdleR", fireBarTex, Vector2(0,0), Vector2(8, 96)
			, Vector2(0,0), 0.1f,1);
		fireBarAnimator->PlayAnimation(L"IdleR");

	}
	void Stage3::CreateLavaBubble(GameObject* lavabubble, float xSize, float ySize, float xOffset, float yOffset, float StartTime,float launchHeight)
	{

		LavaBubbleScript* lavabubblescript = lavabubble->AddComponent<LavaBubbleScript>();
		lavabubblescript->SetLauchStartTime(StartTime);
		lavabubblescript->SetLauchHeight(launchHeight);

		Rigidbody* rb = lavabubble->AddComponent<Rigidbody>();
		rb->SetNeedGravity(false);

		Transform* lavabubbleTr = lavabubble->GetComponent<Transform>();
		lavabubbleTr->SetScale(Vector2(0.07, 0.07));
		lavabubblescript->SetLauchPos(lavabubbleTr->GetPosition());

		BoxCollider2D* lavabubbleCol = lavabubble->AddComponent<BoxCollider2D>();
		lavabubbleCol->SetSize(Vector2(xSize, ySize));
		lavabubbleCol->SetOffset(Vector2(xOffset, yOffset));

		Vector2 pos = lavabubbleTr->GetPosition();
		Vector2 colOffset = lavabubbleCol->GetOffset();

		Animator* lavabubbleAnimator = lavabubble->AddComponent<Animator>();

		graphics::Texture* lavaBubbleUpTex = Resources::Find<graphics::Texture>(L"LAVABUBBLEUP");
		graphics::Texture* lavaBubbleDownTex = Resources::Find<graphics::Texture>(L"LAVABUBBLEDOWN");

		lavabubbleAnimator->CreateAnimation(L"UpR", lavaBubbleUpTex, Vector2(0, 0), Vector2(224, 256)
			, Vector2(107, 175), 0.05f, 1);
		lavabubbleAnimator->CreateAnimation(L"DownR", lavaBubbleDownTex, Vector2(0, 0), Vector2(224, 256)
			, Vector2(107, 175), 0.05f, 1);

	}

	void Stage3::CreateBridgeBlocks()
	{
		//2257
		Block* axe = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2267, 127));
		axe->AddComponent<AxeScript>();
		Animator* axeAnimator = axe->AddComponent<Animator>();
		BoxCollider2D* axeCol = axe->AddComponent<BoxCollider2D>();
		axeCol->SetSize(Vector2(0.18f, 0.18f));
		axeCol->SetOffset(Vector2(-10, 0));

		graphics::Texture* tex = Resources::Find<graphics::Texture>(L"STAGE3MAPS");

		axeAnimator->CreateAnimation(L"IdleR", tex, Vector2(0,18), Vector2(18, 18), Vector2(0, 0), 0.1f, 1);

		Block* block1 = object::Instantiate<Block>(enums::eLayerType::Block,Vector2(2248,160));
		
		BoxCollider2D* col = block1->AddComponent<BoxCollider2D>();
		col->SetSize(Vector2(0.19, 0.17));
		col->SetOffset(Vector2(-10, 1));

		float intervalTime = 0.15f;
		BridgeBlockScript* bscript = block1->AddComponent<BridgeBlockScript>();
		bscript->SetVanishInterval(intervalTime);
		
		Animator* anim = block1->AddComponent<Animator>();
		anim->CreateAnimation(L"IdleR", tex, Vector2(9, 53), Vector2(19, 32), Vector2(0, 0), 0.1f, 1);
		anim->PlayAnimation(L"IdleR");

		for (int i = 1; i <= 10; i++)
		{
			Block* block = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2248-(19*i), 170));
			
			Animator* anim = block->AddComponent<Animator>();
			anim->CreateAnimation(L"IdleR", tex, Vector2(9, 69), Vector2(19, 17), Vector2(0, 0), 0.1f, 1);
			anim->PlayAnimation(L"IdleR");

			BoxCollider2D* col = block->AddComponent<BoxCollider2D>();
			col->SetSize(Vector2(0.19, 0.17));
			col->SetOffset(Vector2(-10, -9));

			BridgeBlockScript* bscript = block->AddComponent<BridgeBlockScript>();
			bscript->SetVanishInterval(intervalTime * (i+1));

		

		}

	}

	void Stage3::MakeFloor()
	{

		Floor* floor1 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(0, 160));
		CreateFloor(floor1, 2.08, 0.8);

		Floor* floor2 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(238, 160));
		CreateFloor(floor2, 1.76, 0.8);

		Floor* floor3 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(462, 160));
		CreateFloor(floor3, 0.47, 0.8);

		Floor* floor4 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(558, 144));
		CreateFloor(floor4, 5.91, 0.93);

		Floor* floor5 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1149, 160));
		CreateFloor(floor5, 5.11, 0.93);

		Floor* floor6 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1666, 207));
		CreateFloor(floor6, 3.84, 0.93);

		Floor* floor7 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1858, 160));
		CreateFloor(floor7, 0.64, 0.93);

		Floor* floor8 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1968, 160));
		CreateFloor(floor8, 0.80, 0.93);

		Floor* floor9 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2255, 144));
		CreateFloor(floor9, 0.48, 0.93);

		Floor* floor10 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2255, 207));
		CreateFloor(floor10, 3.04, 0.93);

		CreateStair(Vector2(0, 144), 5, 3, 'L');
	}
	void Stage3::MakeCeiling()
	{
		Block* ceiling1 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(0, 33));
		CreateCeiling(ceiling1, 3.66, 0.46);

		Block* ceiling = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(368, 33));
		CreateCeiling(ceiling, 0.16, 0.63);

		Block* ceiling2 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(384, 33));
		CreateCeiling(ceiling2, 2.08, 0.16);

		Block* ceiling3 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(591, 33));
		CreateCeiling(ceiling3, 5.60, 0.62);

		Block* ceiling4 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1151, 33));
		CreateCeiling(ceiling4, 3.98, 0.16);

		Block* ceiling5 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1549, 33));
		CreateCeiling(ceiling5, 1.22, 0.46);

		Block* ceiling6 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1670, 33));
		CreateCeiling(ceiling6, 3.00, 0.16);

		Block* ceiling7 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1970, 33));
		CreateCeiling(ceiling7, 0.80, 0.46);

		Block* ceiling8 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2050, 33));
		CreateCeiling(ceiling8, 2.24, 0.16);

		Block* ceiling9 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2274, 33));
		CreateCeiling(ceiling9, 0.32, 0.63);

		Block* ceiling10 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2306, 33));
		CreateCeiling(ceiling10, 2.56, 0.16);
	}
	void Stage3::MakeFireBar()
	{
		Laser* fireBar1 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(376, 104));
		CreateFireBar(fireBar1,0.03,0.46,-1,1.5);

		Laser* fireBar2 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(600, 104));
		CreateFireBar(fireBar2, 0.03, 0.46, -1, 1.5);

		Laser* fireBar3 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(792, 104));
		CreateFireBar(fireBar3, 0.03, 0.46, -1, 1.5,2.0f);

		Laser* fireBar4 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(968, 104));
		CreateFireBar(fireBar4, 0.01, 0.44, -1, 1.5);

		Laser* fireBar5 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(1079, 104));
		CreateFireBar(fireBar5, 0.03, 0.46, -1, 1.5, 2.0f);

		Laser* fireBar6 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(1222, 151));
		CreateFireBar(fireBar6, 0.03, 0.46, -1, 1.5);

		Laser* fireBar7 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(1286, 71));
		CreateFireBar(fireBar7, 0.03, 0.46, -1, 1.5, 2.0f);

		Laser* fireBar8 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(1350, 151));
		CreateFireBar(fireBar8, 0.03, 0.46, -1, 1.5, 2.0f);

		Laser* fireBar9 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(1414, 71));
		CreateFireBar(fireBar9, 0.03, 0.46, -1, 1.5);

		Laser* fireBar10 = object::Instantiate<Laser>(enums::eLayerType::Obstacle, Vector2(1478, 151));
		CreateFireBar(fireBar10, 0.03, 0.46, -1, 1.5, 2.0f);

	}
	void Stage3::MakeLavaBubble()
	{
		LavaBubble* lavabubble1 = object::Instantiate<LavaBubble>(enums::eLayerType::Obstacle, Vector2(220, 132));
		CreateLavaBubble(lavabubble1, 0.15, 0.17, -4, 47);

		LavaBubble* lavabubble2 = object::Instantiate<LavaBubble>(enums::eLayerType::Obstacle, Vector2(434, 152));
		CreateLavaBubble(lavabubble2, 0.15, 0.17, -4, 47,2.0f,200.0f);

		LavaBubble* lavabubble3 = object::Instantiate<LavaBubble>(enums::eLayerType::Obstacle, Vector2(531, 152));
		CreateLavaBubble(lavabubble3, 0.15, 0.17, -4, 47,2.5f,200.0f);
	}

	void ME::Stage3::playerInitialize()
	{
		PlayerScript* playerScript = mPlayer->AddComponent<PlayerScript>();

		playerScript->SetEffect(mEffect);

		PlayerScript::ReSetHp(100.0f);


		mPlayer->GetComponent<Transform>()->SetScale(Vector2(0.45f, 0.45f));
		mPlayer->GetComponent<Transform>()->SetName(L"Player");

		mPlayer->AddComponent <Rigidbody>();

		mPlayer->AddComponent<AudioListener>();
		mPlayer->AddComponent<AudioSource>();

		Rigidbody* playerRb = mPlayer->GetComponent<Rigidbody>();
		playerRb->SetMass(1.0f);

		BoxCollider2D* playerBoxCollider = mPlayer->AddComponent<BoxCollider2D>();
		playerBoxCollider->SetName(L"Player");

		playerBoxCollider->SetOffset(Vector2(-17, -20));
		playerBoxCollider->SetSize(Vector2(0.08f, 0.17f));

		//object::DontDestroyOnLoad(mPlayer);


		graphics::Texture* megamanRightTex = Resources::Find<graphics::Texture>(L"MEGAMANR");
		graphics::Texture* megamanLeftTex = Resources::Find<graphics::Texture>(L"MEGAMANL");

		Animator* animator = mPlayer->AddComponent<Animator>();
		CreatePlayerAnimation(animator, megamanRightTex, megamanLeftTex);

		animator->GetCompleteEvent(L"JumpR") = std::bind(&PlayerScript::PlayStandingAnimByPrevDirection, playerScript);
		animator->GetCompleteEvent(L"JumpL") = std::bind(&PlayerScript::PlayStandingAnimByPrevDirection, playerScript);


		animator->PlayAnimation(L"StandingR", true);


		animator->GetCompleteEvent(L"StandAttackL") = std::bind(&PlayerScript::MakeBullet, playerScript, false);
		animator->GetCompleteEvent(L"StandAttackR") = std::bind(&PlayerScript::MakeBullet, playerScript, false);
	}

}