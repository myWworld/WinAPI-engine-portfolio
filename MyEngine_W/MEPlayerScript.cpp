#include "MEPlayerScript.h"
#include "MEInput.h"
#include "METransform.h"
#include "METime.h"
#include "MEAudioSource.h"
#include "MEAudioClip.h"

#include "MEGameObject.h"
#include "MEMonsters.h"

#include "MEBulletScript.h"
#include "MEBullet.h"
#include "MEObject.h"
#include "MEResources.h"
#include "MEBoxCollider2D.h"
#include "MESceneManager.h"
#include "MEScenes.h"

#include "CommonInclude.h"

#include "MERenderer.h"

#include "METurtle.h"
#include "METurtleScript.h"
#include "MEAxeScript.h"


namespace ME
{
	int PlayerScript::mScore = 0;
	float PlayerScript::mHp = 100.0f;
	bool PlayerScript::mbIsStar = false;
	bool PlayerScript::mbIsHurtState = false;

	PlayerScript::PlayerScript()
		:isJump(false)
		, jumpSeconds(0)
		, mState(PlayerScript::eState::Standing)
		, mAnimator(nullptr)
		, mEffect(nullptr)
		, mStarTime(0.0f)
		, mAttackTime(0.0f)
		, mbIsRunningAttack(false)
		, mbStillStartTime(false)	
		, mbIsOnFlag(false)
		, mAxeTime(0.0f)
		, mbIsAxeGone(false)
		, mHurtCount(0.0f)
		, mbHurtCountStart(false)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{

	}
	void PlayerScript::Update()
	{

		if (mAnimator == nullptr)
		{
			mAnimator = GetOwner()->GetComponent<Animator>();
		}

		if (mbIsHurtState)
		{
			mState = eState::Hurt;

			if(mbHurtCountStart == false)
				mbHurtCountStart = true;
		}

		if (mbHurtCountStart == true)
		{
			mHurtCount += Time::DeltaTime();

			if (mHurtCount > 3.0f)
			{
				mHurtCount = 0.0f;
				mbHurtCountStart = false;
			}
		}



		IsDie();
		IsStarMode();

		IsAxeBroken();

		switch (mState)
		{
		case ME::PlayerScript::eState::Standing:
			Standing();
			break;
		case ME::PlayerScript::eState::StandAttack:
			StandingAttack();
			break;
		case ME::PlayerScript::eState::RunningAttack:
			RunningAttack();
			break;
		case ME::PlayerScript::eState::Jump:
			Jump();
			break;
		case ME::PlayerScript::eState::Run:
			Run();
			break;
		case ME::PlayerScript::eState::Walk:
			Move();
			break;
		case ME::PlayerScript::eState::Fall:
			Move();
			break;
		case ME::PlayerScript::eState::GetDown:
			Move();
			break;
		case ME::PlayerScript::eState::Die:
			Die();
			break;
		case ME::PlayerScript::eState::Hurt:
			Hurt();
			break;
		case ME::PlayerScript::eState::Clear:
			StageClear();
			break;
		default:
			break;
		}


	}

	void PlayerScript::LateUpdate()
	{
	}

	void PlayerScript::Render(HDC hdc)
	{
		PrintScore(hdc);
	}


	void PlayerScript::OnCollisionEnter(Collider* other)
	{
		if (mbIsHurtState == true || mbHurtCountStart == true)
			return;

	

		if (other->GetOwner()->GetLayerType() == enums::eLayerType::Monster)
		{
			Monsters* monster = static_cast<Monsters*>(other->GetOwner());
			enums::eLayerType layertype = other->GetOwner()->GetLayerType();

			if (other->GetName() == L"Cannon"
				 || monster->IsMonsterDead()
					|| mbIsOnFlag == true)
			{
				return;
			}

			if (other->GetName() == L"Head")
			{
				return;
			}
			
			if (mbIsStar == true && layertype == enums::eLayerType::Monster)
			{
				Rigidbody* rb = other->GetOwner()->GetComponent<Rigidbody>();

				Vector2 velocity = rb->GetVelocity();
				velocity.y = -300.0f;
				rb->SetVelocity(velocity);
				rb->SetGround(false);
			
				if (other->GetName() == L"Cannon")
				{
					rb->SetNeedGravity(true);
					other->GetOwner()->GetComponent<Transform>()->SetRotation(25);
				}

				return;
			}

			if (mHp != 0)
			{
				GetHurtAccordingToHp(monster);
			
			}
			else if(mHp == 0)
				return;
		}

		if (other->GetOwner()->GetLayerType() == enums::eLayerType::Obstacle)
		{
			GameObject* obstacle = other->GetOwner();

			if (mHp != 0)
			{
				GetHurtAccordingToHp(obstacle);

			}
			else if (mHp == 0)
				return;
		}

		if (other->GetName() == L"Star")
		{

			if (mbIsStar == true)
			{
				mStarTime = 0.0f;
			}
			else
			{
				mbIsStar = true;

				if(mEffect == nullptr)
					mEffect = CreateAura();
			}
	
	
		}

		if (other->GetName() == L"Flag")
		{
			mbIsOnFlag = true;
			mState = eState::Clear;
			mAnimator->PlayAnimation(L"RightWalkR");
		}

		if (other->GetName() == L"LastDoor")
		{
			mAnimator->PlayAnimation(L"ClearL", false);
			mbIsOnFlag = false;

		}
		
	}

	void PlayerScript::OnCollisionStay(Collider* other)
	{
		
	}
	void PlayerScript::OnCollisionExit(Collider* other)
	{
	}
	void PlayerScript::Standing()
	{

		if (Input::GetKey(eKeyCode::Right) || Input::GetKey(eKeyCode::D))
		{

			mState = eState::Walk;
			mAnimator->PlayAnimation(L"RightWalkR");

		}


		if (Input::GetKey(eKeyCode::Left) || Input::GetKey(eKeyCode::A))
		{

			mState = eState::Walk;
			mAnimator->PlayAnimation(L"LeftWalkL");

		}

		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

		if (Input::GetKey(eKeyCode::Space) && rb->IsGround())
		{
				PlayJumpAnimationByPrevDirection();
		}

		if (Input::GetKey(eKeyCode::T))
		{
			mState = eState::StandAttack;

			if (mPrevDirection == ePrevDirection::Left)
				mAnimator->PlayAnimation(L"StandAttackL");

			if (mPrevDirection == ePrevDirection::Right)
				mAnimator->PlayAnimation(L"StandAttackR");
		}


	}

	void PlayerScript::Move()
	{


		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 velocity = rb->GetVelocity();


		if (Input::GetKey(eKeyCode::Right) || Input::GetKey(eKeyCode::D))
		{

			mPrevDirection = ePrevDirection::Right;

			DetermineMovingVelocity();

			//rb->AddForce(Vector2(130, 0)); //이동 방향으로 


			if (Input::GetKey(eKeyCode::T))
			{
				mState = eState::RunningAttack;
				mAnimator->PlayAnimation(L"RunningAttackR");
			}


		}

		if (Input::GetKey(eKeyCode::Left) || Input::GetKey(eKeyCode::A))
		{
			mPrevDirection = ePrevDirection::Left;

			DetermineMovingVelocity();

			//rb->AddForce(Vector2(-10, 0));

			//if (Input::GetKey(eKeyCode::Shift)) //run는 이 게임에선 필요없을 듯
		//{
		//	mState = eState::Run; 
		//	mAnimator->PlayAnimation(L"RunL");
		//}

			if (Input::GetKey(eKeyCode::T))
			{
				mState = eState::RunningAttack;
				mAnimator->PlayAnimation(L"RunningAttackL");
			}

		}


		if (Input::GetKey(eKeyCode::Space) && rb->IsGround())
		{
			PlayJumpAnimationByPrevDirection();
		}

		tr->SetPosition(pos);

		if ((Input::GetKeyUp(eKeyCode::Right) || Input::GetKeyUp(eKeyCode::D))
			|| (Input::GetKeyUp(eKeyCode::Left) || Input::GetKeyUp(eKeyCode::A))
			|| (Input::GetKeyUp(eKeyCode::Space)))
		{
			mState = eState::Standing;

			PlayStandingAnimByPrevDirection();
		}
	}

	void PlayerScript::DetermineMovingVelocity()
	{
		Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 velocity = playerRb->GetVelocity();


		if (mPrevDirection == ePrevDirection::Left)
		{
			if (velocity.x > 0)
			{
				playerRb->AddForce(Vector2(-300, 0)); //움직임 전환할 때 반대로 전환을 빠르게 하기 위해 속도 조절
			}
			else
			{
				playerRb->AddForce(Vector2(-150, 0));
			}
		}
		if (mPrevDirection == ePrevDirection::Right)
		{
			if (velocity.x < 0)
			{
				playerRb->AddForce(Vector2(300, 0));
			}
			else
			{
				playerRb->AddForce(Vector2(150, 0));
			}
		}
	}

	void PlayerScript::Jump()
	{

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

		//AudioSource* as = GetOwner()->GetComponent<AudioSource>();
		//AudioClip* ac = GetOwner()->GetComponent<AudioClip>();
		//ac = Resources::Find<AudioClip>(L"MARIOJUMPSOUND");
		//
		//if (ac == nullptr)
		//	return;
		//
		//as->SetClip(ac);
		//
		//if (as == nullptr)
		//	return;
		//
		//as->Play();

		Vector2 velocity = rb->GetVelocity();
		velocity.y = -325.0f;
		rb->SetVelocity(velocity);

		rb->SetGround(false);


		mState = eState::Standing;

		

	}

	void PlayerScript::Run()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();
		


		if (mPrevDirection == ePrevDirection::Left)
		{
		
		}
		else if (mPrevDirection == ePrevDirection::Right)
		{
			pos += Vector2::Right * (130 * Time::DeltaTime());
		}


		if (Input::GetKey(eKeyCode::Space) && rb->IsGround())
		{
			PlayJumpAnimationByPrevDirection();
		}



		tr->SetPosition(pos);

		if ((Input::GetKeyUp(eKeyCode::Right) || Input::GetKeyUp(eKeyCode::D))
			|| (Input::GetKeyUp(eKeyCode::Left) || Input::GetKeyUp(eKeyCode::A))
			|| Input::GetKeyUp(eKeyCode::Shift))
		{
			mState = eState::Standing;
			PlayStandingAnimByPrevDirection();
		}
	}

	void PlayerScript::Die()
	{
		GetOwner()->SetActive(false);
		GetOwner()->SetDeath();

		renderer::mainCamera->SetTarget(nullptr);


		SceneManager::LoadScene(L"GameOverScene");
	}

	void PlayerScript::Hurt()
	{

		if (mAnimator->IsComplete())
		{
			Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();
			playerRb->StopMoving();

			mState = eState::Standing;
			PlayStandingAnimByPrevDirection();
			mbIsHurtState = false;
		}
	}

	void PlayerScript::StageClear()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

		Vector2 velocity = rb->GetVelocity();

		if (velocity != Vector2::Zero)
			rb->StopMoving();

		rb->SetNeedGravity(true, Vector2(0, 1200));

		Vector2 pos = tr->GetPosition();

		pos.x += 70 * Time::DeltaTime();

	
		if(mbIsOnFlag == true)
			tr->SetPosition(pos);

	}

	void PlayerScript::StandingAttack()
	{
		if (Input::GetKeyUp(eKeyCode::T))
		{
			mState = eState::Standing;
			PlayStandingAnimByPrevDirection();
		}
		if (Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::D))
		{
			mState = eState::RunningAttack;


			if (Input::GetKey(eKeyCode::A))
			{
				mAnimator->PlayAnimation(L"RunningAttackL");
			}

			if (Input::GetKey(eKeyCode::D))
			{
				mAnimator->PlayAnimation(L"RunningAttackR");
			}
		}
	}

	void PlayerScript::RunningAttack()
	{

		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (Input::GetKey(eKeyCode::Right) || Input::GetKey(eKeyCode::D))
		{
			mPrevDirection = ePrevDirection::Right;
			rb->AddForce(Vector2(130, 0));
			RunAttackTime();

			if (Input::GetKey(eKeyCode::Space) && rb->IsGround())
			{

				mState = eState::Jump;
				mAnimator->PlayAnimation(L"JumpR", false);

			}
		}

		if (Input::GetKey(eKeyCode::Left) || Input::GetKey(eKeyCode::A))
		{
			mPrevDirection = ePrevDirection::Left;
			rb->AddForce(Vector2(-130, 0));
			RunAttackTime();

			if (Input::GetKey(eKeyCode::Space) && rb->IsGround())
			{

				mState = eState::Jump;
				mAnimator->PlayAnimation(L"JumpR", false);

			}
		}

	


		if (Input::GetKeyUp(eKeyCode::T))
		{
			mState = eState::Standing;
			PlayStandingAnimByPrevDirection();
		}

		if (Input::GetKeyUp(eKeyCode::A) || Input::GetKeyUp(eKeyCode::D))
		{
			mState = eState::StandAttack;

			if (mPrevDirection == ePrevDirection::Left)
				mAnimator->PlayAnimation(L"StandAttackL");

			if (mPrevDirection == ePrevDirection::Right)
				mAnimator->PlayAnimation(L"StandAttackR");
		}

	}

	void PlayerScript::MakeBullet(bool isRunning)
	{
		GameObject* bullet = object::Instantiate<Bullet>(enums::eLayerType::Bullet);

		BulletScript* bulletScript = bullet->AddComponent<BulletScript>();
		bulletScript->SetPlayerIsRunningAttack(isRunning);

		Animator* bulletAnim = bullet->AddComponent<Animator>();

		BoxCollider2D* bulletCollider = bullet->AddComponent<BoxCollider2D>();
		bulletCollider->SetName(L"Bullet");

		bulletCollider->SetSize(Vector2(0.039f, 0.039f));
		bulletCollider->SetOffset(Vector2(-17, -20));

		graphics::Texture* bulletRightTex = Resources::Find<graphics::Texture>(L"BULLETR");
		graphics::Texture* bulletLeftTex = Resources::Find<graphics::Texture>(L"BULLETL");


		Transform* bulletTr = bullet->GetComponent<Transform>();

		Transform* playerTr = GetOwner()->GetComponent<Transform>();
		Vector2 playerPos = playerTr->GetPosition();

		Vector2 bulletPos = Vector2::Zero;

		bulletAnim->CreateAnimation(L"BulletR", bulletRightTex, Vector2(0, 0)
			, Vector2(50, 50), Vector2(0, 0), 0.3f, 1);
		bulletAnim->CreateAnimation(L"BulletL", bulletLeftTex, Vector2(0, 0)
			, Vector2(50, 50), Vector2(0, 0), 0.3f, 1);

		if (mPrevDirection == ePrevDirection::Left)
		{
			bulletPos = playerTr->GetPosition() + Vector2(-10, 4);
		}
		else if (mPrevDirection == ePrevDirection::Right)
		{
			bulletPos = playerTr->GetPosition() + Vector2(18, 4);
		}

		bulletTr->SetPosition(bulletPos);
		bulletTr->SetScale(Vector2(0.2f, 0.2f));

		PlayBulletByPrveDirection(bulletAnim);

	}

	void PlayerScript::RunAttackTime()
	{
		mAttackTime += Time::DeltaTime();

		if (mAttackTime > 0.4f)
		{
			MakeBullet(true);
			mAttackTime = 0.0f;
		}
	}

	void PlayerScript::PlayStandingAnimByPrevDirection()
	{
		if (mPrevDirection == ePrevDirection::Left)
		{
			mAnimator->PlayAnimation(L"StandingL");
		}
		else if (mPrevDirection == ePrevDirection::Right)
		{
			mAnimator->PlayAnimation(L"StandingR");
		}
	}

	void PlayerScript::PlayBulletByPrveDirection(Animator* animator)
	{
		Bullet* obj = static_cast<Bullet*>(animator->GetOwner());


		if (mPrevDirection == ePrevDirection::Left)
		{
			animator->PlayAnimation(L"BulletL", true);
			obj->SetDirection(Bullet::eDirection::Left);

		}
		else if (mPrevDirection == ePrevDirection::Right)
		{
			animator->PlayAnimation(L"BulletR", true);
			obj->SetDirection(Bullet::eDirection::Right);
		}
	}

	void PlayerScript::PlayJumpAnimationByPrevDirection()
	{
		mState = eState::Jump;

		if (mPrevDirection == ePrevDirection::Right)
			mAnimator->PlayAnimation(L"JumpR", false);
		else if(mPrevDirection == ePrevDirection::Left)
			mAnimator->PlayAnimation(L"JumpL", false);
	}

	void PlayerScript::PlayHurtAnimationByMonster( float rightOrLeft)
	{
		if (mPrevDirection == ePrevDirection::Left)
		{
			mAnimator->PlayAnimation(L"HurtL",false);

		}
		else if (mPrevDirection == ePrevDirection::Right)
		{
			mAnimator->PlayAnimation(L"HurtR", false);

		}

		mbIsHurtState = true;

	}

	void PlayerScript::IsDie()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (mHp == 0 )//|| pos.y >= 280)
		{
			mState = eState::Die;
		}
	}
	
	void PlayerScript::IsStarMode()
	{
		if (mbIsStar == true)
		{
			mStarTime += Time::DeltaTime();
			PlayAuraAnimation();

			if (mStarTime > 10.0f)
			{
				mStarTime = 0.0f;
				mbIsStar = false;
				mEffect->SetDeath();
			}
		}
	}

	void PlayerScript::IsAxeBroken()
	{
		if (AxeScript::IsTrigger() && mbIsAxeGone == false)
		{
			mAxeTime += Time::DeltaTime();
			mState = eState::End;
			Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();
			rb->SetNeedGravity(false);

			if (mAxeTime > 1.7f)
			{
				rb->SetNeedGravity(true, Vector2(0.0f, 800.0f));
				rb->StopMoving();
				mState = eState::Standing;
				mbIsAxeGone = true;
			}
		}
	}
	
	float PlayerScript::DetermineLeftOrRightByVector(GameObject *obj)
	{
		Transform* objTr = obj->GetComponent<Transform>();

		Transform* playerTr = GetOwner()->GetComponent<Transform>();
		Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();

		Vector2 playerColPos = playerTr->GetPosition() + GetOwner()->GetComponent<Collider>()->GetOffset();
		Vector2 monsterColPos = objTr->GetPosition() + obj->GetComponent<Collider>()->GetOffset();

		Vector2 playerColliderSize = GetOwner()->GetComponent<Collider>()->GetSize() * 100.0f;
		Vector2 monsterColliderSize = obj->GetComponent<Collider>()->GetSize() * 100.0f;

		Vector2 playerCenterColPos = playerColPos + (playerColliderSize / 2.0f);
		Vector2 monsterCenterColPos = monsterColPos + (monsterColliderSize / 2.0f);

		Vector2 leftOrRight = playerCenterColPos - monsterCenterColPos;

		return leftOrRight.x;
	}

	void PlayerScript::PrintScore(HDC hdc)
	{
		std::wstring wstr = std::to_wstring(mScore);


		HFONT hfont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("jejufont"));
		HFONT oldfond = (HFONT)SelectObject(hdc, (HFONT)hfont);

		SelectObject(hdc, oldfond);
		DeleteObject(hfont);

		wchar_t str[50];
		wcsncpy_s(str, wstr.c_str(), 5);

		SetTextColor(hdc, 0x0000FF00);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 700, 0, str, wcslen(str));

	}
	void PlayerScript::PlayAuraAnimation()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		if (mEffect == nullptr)
			return;

		Animator* effectAnimator = mEffect->GetComponent<Animator>();

		Transform* effectTr = mEffect->GetComponent<Transform>();
		
		effectTr->SetPosition(Vector2(pos.x + 65, pos.y + 70));
		effectTr->SetScale(Vector2(0.2f, 0.2f));

		
		effectAnimator->PlayAnimation(L"StarEffectR");
	}

	void PlayerScript::GetHurtAccordingToHp(GameObject* obj)
	{
		mHp -= 10;
		Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();
		Vector2 playerVelocity = playerRb->GetVelocity();

		float leftOrRight = DetermineLeftOrRightByVector(obj);

		if (leftOrRight >= 0)
		{
			playerVelocity.x += 50.0f;
		}
		else
		{
			playerVelocity.x -= 50.0f;

		}
		playerVelocity.y -= 100.0f;

		playerRb->SetVelocity(playerVelocity);
		playerRb->SetGround(false);

		PlayHurtAnimationByMonster(leftOrRight);
	}

	GameObject* PlayerScript::CreateAura()
	{
		GameObject* effect = object::Instantiate <GameObject> (enums::eLayerType::Aura);

		Animator* effectAnimator = effect->AddComponent<Animator>();

		graphics::Texture* effectTex = Resources::Find<graphics::Texture>(L"STAREFFECT");

		effectAnimator->CreateAnimation(L"StarEffectR", effectTex, Vector2(0, 0), Vector2(196, 212), Vector2::Zero
			, 0.01f, 5);

		return effect;
	}
}


