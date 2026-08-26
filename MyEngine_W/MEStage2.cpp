#include "MEStage2.h"
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


	ME::Stage2::Stage2()
		:mCannonLaunchTime(0.0f)
	{
	}

	ME::Stage2::~Stage2()
	{
	}

	void ME::Stage2::Initialize()
	{
		{	
			Vector2 resolution = Vector2(application.GetWidth(), application.GetHeight());

			GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, resolution / 2.0f);
			mCameraComp = camera->AddComponent<Camera>();
			mCameraComp->SetMapTotalWidth(2864.0f);
			renderer::mainCamera = mCameraComp;

		}

		{
			GameObject* background = object::Instantiate<GameObject>(enums::eLayerType::BackGround,Vector2(0,0));
			SpriteRenderer* backgroundSr = background->AddComponent<SpriteRenderer>();

			graphics::Texture* backgroundTex = Resources::Find<graphics::Texture>(L"STAGE2");

			backgroundSr->SetTexture(backgroundTex);

		}
		
		Floors();
		MovingBlocks();


		{
			GameObject* qbox1 = object::Instantiate<GameObject>(enums::eLayerType::Block, Vector2(990, 147));
			CreateQbox(qbox1);
		}

		{
			Trampoline* trampoline1 = object::Instantiate<Trampoline>(enums::eLayerType::Block, Vector2(630, 214));
			CreateTrampoline(trampoline1,0.16,0.19,-24,-23);

			Trampoline* trampoline2 = object::Instantiate<Trampoline>(enums::eLayerType::Block, Vector2(1880, 214));
			CreateTrampoline(trampoline2, 0.16, 0.19, -24, -23);
		}

		{
			Flag* flag = object::Instantiate<Flag>(enums::eLayerType::Block, Vector2(2679, 193));
			CreateFlag(flag, 0.04,1.7,0,-143);

			LastDoor* lastdoor = object::Instantiate<LastDoor>(enums::eLayerType::Block, Vector2(2797, 167));
			CreateLastDoor(lastdoor, 0.27, 0.4);
		}

		Scene::Initialize();
	}

	void ME::Stage2::Update()
	{
		mCannonLaunchTime += Time::DeltaTime();

		if (mCannonLaunchTime > 8.0f)
		{
			if (FlagScript::IsOnFlag() == false)
			{
				CreateCannon();
				mCannonLaunchTime = 0.0f;
			}
		}

		Scene::Update();
	}

	void ME::Stage2::LateUpdate()
	{
		if (LastDoorScript::IsClearStage())
		{
			LastDoorScript::SetClearStage(false);
			SceneManager::LoadScene(L"Stage3");
		}
		Scene::LateUpdate();
	}

	void ME::Stage2::Render(HDC mHdc)
	{
		Scene::Render(mHdc);
	}

	void ME::Stage2::OnEnter()
	{
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Particle, enums::eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Particle, enums::eLayerType::Player, true);


		CollisionManager::CollisionLayerCheck(enums::eLayerType::Monster, enums::eLayerType::Floor, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Monster, enums::eLayerType::Block, true);

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Block, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Floor, true);

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Items, enums::eLayerType::Block, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Items, enums::eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Items, enums::eLayerType::Floor, true);

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Bullet, enums::eLayerType::Block, true);

		mPlayer = object::Instantiate<Player>
			(enums::eLayerType::Player, Vector2(150, 100));


		renderer::mainCamera = mCameraComp;
		mCameraComp->SetTarget(mPlayer);

		playerInitialize();
		PlayerScript::ReSetScore(0.0f);
		mPlayTime = 0.0f;

		FlagScript::SetFlag(false);

		UIManager::Push(enums::eUIType::HpBar);
		Scene::OnEnter();
	}

	void ME::Stage2::OnExit()
	{
		UIManager::Pop(enums::eUIType::HpBar);
		Scene::OnExit();
	}

	void ME::Stage2::CreatePlayerAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture)
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

	void ME::Stage2::CreateMushRoomAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture)
	{
	}

	void ME::Stage2::CreateSkeletonAnimation(Animator* animator, graphics::Texture* Rtexture, graphics::Texture* Ltexture)
	{
	}

	void ME::Stage2::CreateMushRoom(GameObject* mushroom)
	{
	}

	void ME::Stage2::CreateSkeleton(GameObject* skeleton)
	{
	}

	void ME::Stage2::CreateCannon()
	{
		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector2 playerPos = playerTr->GetPosition();


		Cannon* cannon = object::Instantiate<Cannon>(enums::eLayerType::Monster, Vector2(playerPos.x + application.GetWidth() / 2.0f, playerPos.y));

		Rigidbody* cannonRb = cannon->AddComponent<Rigidbody>();
		cannonRb->SetNeedGravity(false);

		cannon->GetComponent<Transform>()->SetScale(Vector2(0.3f, 0.3f));

		BoxCollider2D* cannonCol = cannon->AddComponent<BoxCollider2D>();
		CannonScript* cannonScript = cannon->AddComponent<CannonScript>();
		Animator* cannonAnimator = cannon->AddComponent<Animator>();

		cannonCol->SetSize(Vector2(0.19f, 0.097f));
		cannonCol->SetOffset(Vector2(-30, -18));
		cannonCol->SetName(L"Cannon");

		graphics::Texture* cannonTex = Resources::Find<graphics::Texture>(L"CANNON");

		cannonAnimator->CreateAnimation(L"CannonL", cannonTex, Vector2(0, 0), Vector2(60, 40), Vector2::Zero, 0.1f, 1);

		cannonAnimator->PlayAnimation(L"CannonL");
	}

	void ME::Stage2::CreateQbox(GameObject* qbox)
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

	void Stage2::CreateFloor(GameObject* floor, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* floorCol = floor->AddComponent<BoxCollider2D>();
		floor->AddComponent<FloorScript>();
		floorCol->SetName(L"Floor");

		floorCol->SetSize(Vector2(xSize, ySize));
		floorCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage2::CreateLastDoor(GameObject* lastdoor, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* lastDoorCol = lastdoor->AddComponent<BoxCollider2D>();
		lastdoor->AddComponent<LastDoorScript>();
		lastDoorCol->SetName(L"LastDoor");

		lastDoorCol->SetSize(Vector2(xSize, ySize));
		lastDoorCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage2::CreateChimney(GameObject* chimney, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* chimneyCol = chimney->AddComponent<BoxCollider2D>();
		chimney->AddComponent<ChimneyScript>();
		chimneyCol->SetName(L"Chimney");

		chimneyCol->SetSize(Vector2(xSize, ySize));
		chimneyCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage2::CreateStair(Vector2 startPos, int maxSteps, int height, char direction)
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
	void Stage2::CreateFlag(GameObject* flag, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* flagCol = flag->AddComponent<BoxCollider2D>();
		flag->AddComponent<FlagScript>();
		flagCol->SetName(L"Flag");

		flagCol->SetSize(Vector2(xSize, ySize));
		flagCol->SetOffset(Vector2(xOffset, yOffset));
	}
	void Stage2::CreateBlock(GameObject* block, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* BlockCol = block->AddComponent<BoxCollider2D>();
		block->AddComponent<BlockScript>();
		BlockCol->SetName(L"Block");

	
		BlockCol->SetSize(Vector2(xSize, ySize));
		BlockCol->SetOffset(Vector2(xOffset, yOffset));
	}

	void Stage2::CreateMovingBlock(GameObject* block, char direction
		, float maxRangeVal, float minRangeVal
		, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* BlockCol = block->AddComponent<BoxCollider2D>();
		
		

		if (direction == 'H')
		{
			HorizonMovingBlockScript* script = block->AddComponent<HorizonMovingBlockScript>();
			script->SetLength(maxRangeVal, minRangeVal);
		
		}
		else if (direction == 'V')
		{
			VerticalMovingBlockScript* script = block->AddComponent<VerticalMovingBlockScript>();
			script->SetHeight(maxRangeVal,minRangeVal);
		}

		SpriteRenderer* blockSr = block->AddComponent<SpriteRenderer>();
		graphics::Texture* blockTex = Resources::Find<graphics::Texture>(L"MOVINGBLOCK");
		blockSr->SetTexture(blockTex);
		
		BlockCol->SetName(L"Block");
		BlockCol->SetSize(Vector2(xSize, ySize));
		BlockCol->SetOffset(Vector2(xOffset, yOffset));
	}

	void Stage2::CreateTrampoline(GameObject* trampoline
		, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* TrampolineCol = trampoline->AddComponent<BoxCollider2D>();
		Transform* trampolineTr = trampoline->GetComponent<Transform>();

		trampolineTr->SetScale(Vector2(0.4, 0.4));

		TrampolineScript* script = trampoline->AddComponent<TrampolineScript>();
		script->SetoriginOffsetY(yOffset);

		Animator* trampolineAnimator = trampoline->AddComponent<Animator>();
		graphics::Texture* trampolineTex = Resources::Find<graphics::Texture>(L"TRAMPOLINE");
		
		trampolineAnimator->CreateAnimation(L"IdleR", trampolineTex, Vector2(0, 0), Vector2(55, 48), Vector2::Zero, 0.1f, 1);
		trampolineAnimator->CreateAnimation(L"PressedR", trampolineTex, Vector2(0, 0), Vector2(55, 48), Vector2::Zero, 0.2f, 3);

		TrampolineCol->SetName(L"Trampoline");
		TrampolineCol->SetSize(Vector2(xSize, ySize));
		TrampolineCol->SetOffset(Vector2(xOffset, yOffset));

		trampolineAnimator->PlayAnimation(L"IdleR",false);
	}

	void Stage2::CreatePulleyBlock(GameObject* left, GameObject* right
		, float xLeftSize, float yLeftSize, float xRightSize, float yRightSize
		, float xLeftOffset, float yLeftOffset, float xRightOffset, float yRightOffset)
	{
		BoxCollider2D* leftCol = left->AddComponent<BoxCollider2D>();
		Transform* leftTr = left->GetComponent<Transform>();
		Vector2 leftPos = leftTr->GetPosition();

		Rigidbody* leftRb = left->AddComponent<Rigidbody>();
		leftRb->SetNeedGravity(false);


		Rigidbody* rightRb = right->AddComponent<Rigidbody>();
		rightRb->SetNeedGravity(false);

		BoxCollider2D* rightCol = right->AddComponent<BoxCollider2D>();
		Transform* rightTr = right->GetComponent<Transform>();
		Vector2 rightPos = rightTr->GetPosition();

		float minHeight; 
		float maxHeight;
		
		if (leftPos.y < rightPos.y)
		{
			maxHeight = leftPos.y ;
			minHeight = rightPos.y ;
		}
		else
		{
			maxHeight = rightPos.y ;
			minHeight = leftPos.y;
		}

		PulleyBlockScript* leftScript = left->AddComponent<PulleyBlockScript>();
		leftScript->SetOtherSide(right);

		leftScript->SetHeight(maxHeight, minHeight);

		PulleyBlockScript* rightScript = right->AddComponent<PulleyBlockScript>();
		rightScript->SetOtherSide(left);

		rightScript->SetHeight(maxHeight, minHeight);

		graphics::Texture* leftTex = Resources::Find<graphics::Texture>(L"MOVINGBLOCK");
		graphics::Texture* rightTex = Resources::Find<graphics::Texture>(L"MOVINGBLOCK");

		SpriteRenderer* leftSr = left->AddComponent<SpriteRenderer>();
		leftSr->SetTexture(leftTex);
	
		SpriteRenderer* rightSr = right->AddComponent<SpriteRenderer>();
		rightSr->SetTexture(rightTex);

		leftCol->SetName(L"LeftBlock");
		leftCol->SetSize(Vector2(xLeftSize, yLeftSize));
		leftCol->SetOffset(Vector2(xLeftOffset, yLeftOffset));

		rightCol->SetName(L"RightBlock");
		rightCol->SetSize(Vector2(xRightSize, yRightSize));
		rightCol->SetOffset(Vector2(xRightOffset, yRightOffset));
	}


	void Stage2::CreateOnlyDownBlock(GameObject* block
		, float xSize, float ySize, float xOffset, float yOffset)
	{
		BoxCollider2D* BlockCol = block->AddComponent<BoxCollider2D>();

		OnlyMovingDownScript* script = block->AddComponent<OnlyMovingDownScript>();

		SpriteRenderer* blockSr = block->AddComponent<SpriteRenderer>();
		graphics::Texture* blockTex = Resources::Find<graphics::Texture>(L"MOVINGBLOCK");
		blockSr->SetTexture(blockTex);

		BlockCol->SetName(L"DownBlock");
		BlockCol->SetSize(Vector2(xSize, ySize));
		BlockCol->SetOffset(Vector2(xOffset, yOffset));
	}

	void ME::Stage2::playerInitialize()
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

	void Stage2::MovingBlocks()
	{
		Block* verticalMovingBlock1 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(448, 208));
		CreateMovingBlock(verticalMovingBlock1, 'V', 155, 215, 0.3, 0.07);

		Block* verticalMovingBlock2 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(960, 198));
		CreateMovingBlock(verticalMovingBlock2, 'V', 104, 215, 0.3, 0.07);

		Block* horizontalMovingBlock1 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(687, 88));
		CreateMovingBlock(horizontalMovingBlock1, 'H', 704, 630, 0.3, 0.07);

		Block* horizontalMovingBlock2 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(768, 118));
		CreateMovingBlock(horizontalMovingBlock2, 'H', 802, 719, 0.3, 0.07);

		Block* horizontalMovingBlock3 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(880, 104));
		CreateMovingBlock(horizontalMovingBlock3, 'H', 904, 825, 0.3, 0.07);

		Block* horizontalMovingBlock4 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1937, 85));
		CreateMovingBlock(horizontalMovingBlock4, 'H', 1951, 1874, 0.3, 0.07);


		Block* pulleyLeftBlock1 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1134, 83));
		Block* pulleyRightBlock1 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1191, 154));
		CreatePulleyBlock(pulleyLeftBlock1, pulleyRightBlock1,
			0.3, 0.07, 0.3, 0.07);

		Block* pulleyLeftBlock2 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1262, 83));
		Block* pulleyRightBlock2 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(1307, 154));
		CreatePulleyBlock(pulleyLeftBlock2, pulleyRightBlock2,
			0.3, 0.07, 0.3, 0.07);

		Block* pulleyLeftBlock3 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2024, 98));
		Block* pulleyRightBlock3 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2071, 147));
		CreatePulleyBlock(pulleyLeftBlock3, pulleyRightBlock3,
			0.3, 0.07, 0.3, 0.07);

		Block* onlyDownBlock1 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2256, 108));
		CreateOnlyDownBlock(onlyDownBlock1, 0.3, 0.07);

		Block* onlyDownBlock2 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2320, 92));
		CreateOnlyDownBlock(onlyDownBlock2, 0.3, 0.07);

		Block* onlyDownBlock3 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2385, 124));
		CreateOnlyDownBlock(onlyDownBlock3, 0.3, 0.07);

		Block* onlyDownBlock4 = object::Instantiate<Block>(enums::eLayerType::Block, Vector2(2449, 108));
		CreateOnlyDownBlock(onlyDownBlock4, 0.3, 0.07);
	}

	void Stage2::Floors()
	{
		Floor* floor = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(0, 210));
		CreateFloor(floor, 2.56);

		Floor* whiteFloor1 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(289, 210));
		CreateFloor(whiteFloor1, 0.48, 0.04);
		Floor* whiteFloor2 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(337, 147));
		CreateFloor(whiteFloor2, 0.48, 0.04);
		Floor* whiteFloor3 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(385, 210));
		CreateFloor(whiteFloor3, 0.48, 0.04);

		Floor* whiteFloor4 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(496, 147));
		CreateFloor(whiteFloor4, 0.62, 0.04);

		Floor* whiteFloor5 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(593, 210));
		CreateFloor(whiteFloor5, 0.48, 0.04);
		Floor* whiteFloor6 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(689, 210));
		CreateFloor(whiteFloor6, 0.48, 0.04);

		Floor* whiteFloor7 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(785, 180));
		CreateFloor(whiteFloor7, 0.62, 0.04);

		Floor* whiteFloor8 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1041, 147));
		CreateFloor(whiteFloor8, 0.80, 0.04);

		Floor* whiteFloor9 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1363, 163));
		CreateFloor(whiteFloor9, 0.62, 0.04);
		Floor* whiteFloor10 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1363, 50));
		CreateFloor(whiteFloor10, 0.48, 0.04);

		Floor* whiteFloor11 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1443, 131));
		CreateFloor(whiteFloor11, 0.48, 0.04);

		Floor* whiteFloor12 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1489, 210));
		CreateFloor(whiteFloor12, 0.48, 0.04);
		Floor* whiteFloor13 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1555, 210));
		CreateFloor(whiteFloor13, 0.62, 0.04);
		Floor* whiteFloor14 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1650, 210));
		CreateFloor(whiteFloor14, 0.48, 0.04);


		Floor* whiteFloor15 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1713, 148));
		CreateFloor(whiteFloor15, 0.79, 0.04);

		Floor* whiteFloor16 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1809, 210));
		CreateFloor(whiteFloor16, 0.79, 0.04);
		Floor* whiteFloor17 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(1969, 210));
		CreateFloor(whiteFloor17, 0.48, 0.04);

		Floor* whiteFloor18 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2113, 210));
		CreateFloor(whiteFloor18, 0.60, 0.04);
		Floor* whiteFloor19 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2113, 84));
		CreateFloor(whiteFloor19, 0.48, 0.04);

		Floor* whiteFloor20 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2159, 148));
		CreateFloor(whiteFloor20, 0.60, 0.04);

		Floor* whiteFloor21 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2494, 84));
		CreateFloor(whiteFloor21, 0.48, 0.04);

		Floor* floor1 = object::Instantiate<Floor>(enums::eLayerType::Floor, Vector2(2564, 210));
		CreateFloor(floor1, 3);
	}

}