#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_KICK			1
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601


#pragma region ANIMATION_ID

// TANUKI
#define ID_ANI_MARIO_TANUKI_IDLE_RIGHT 20400
#define ID_ANI_MARIO_TANUKI_IDLE_LEFT 20401
#define ID_ANI_MARIO_TANUKI_KICK_RIGHT 20402
#define ID_ANI_MARIO_TANUKI_KICK_LEFT 20403

#define ID_ANI_MARIO_TANUKI_WALKING_RIGHT 20500
#define ID_ANI_MARIO_TANUKI_WALKING_LEFT 20501

#define ID_ANI_MARIO_TANUKI_RUNNING_RIGHT 20600
#define ID_ANI_MARIO_TANUKI_RUNNING_LEFT 20601

#define ID_ANI_MARIO_TANUKI_JUMP_WALK_RIGHT 20700
#define ID_ANI_MARIO_TANUKI_JUMP_WALK_LEFT 20701

#define ID_ANI_MARIO_TANUKI_JUMP_RUN_RIGHT 20800
#define ID_ANI_MARIO_TANUKI_JUMP_RUN_LEFT 20801

#define ID_ANI_MARIO_TANUKI_SIT_RIGHT 20900
#define ID_ANI_MARIO_TANUKI_SIT_LEFT 20901

#define ID_ANI_MARIO_TANUKI_BRACE_RIGHT 21000
#define ID_ANI_MARIO_TANUKI_BRACE_LEFT 21001

// BIG MARIO
#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401
#define ID_ANI_MARIO_KICK_RIGHT 402
#define ID_ANI_MARIO_KICK_LEFT 403

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1103
#define ID_ANI_MARIO_SMALL_KICK_LEFT 1104

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TANUKI	3

#define MARIO_TANUKI_BBOX_WIDTH  14
#define MARIO_TANUKI_BBOX_HEIGHT 24
#define MARIO_TANUKI_SITTING_BBOX_WIDTH  14
#define MARIO_TANUKI_SITTING_BBOX_HEIGHT 16

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16
#define MARIO_BIG_OFFSET_Y_SITTING 6
#define MARIO_BIG_OFFSET_Y_IDLE 2
#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	// Holding related
	LPGAMEOBJECT holdItem;
	bool isHolding;
	const float HOLD_X_OFFSET = -2.0f;
	const float HOLD_Y_OFFSET = -2.0f;

	// Kicking related
	const int KICH_TIMEOUT = 160;
	ULONGLONG kickStart;
	bool isKicking;
	
	BOOLEAN isSitting;
	float maxVx;

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;

	bool isStopUpdate; 

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLifeMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithVenueFireTrap(LPCOLLISIONEVENT e);
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithFireball(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);

	int GetAniIdTanuki();
	int GetAniIdBig();
	int GetAniIdSmall();

	void FireGainPointEvent(float x, float y, int point=100);

public:
	// Constructors
	CMario(float x, float y) : CGameObject(x, y)
	{
		holdItem		  = NULL;
		isHolding		  = false;
		isKicking		  = false;
		isSitting		  = false;
		maxVx			  = 0.0f;
		ax				  = 0.0f;
		ay				  = MARIO_GRAVITY; 
		level			  = MARIO_LEVEL_BIG;
		untouchable		  = 0;
		untouchable_start = -1;
		isOnPlatform	  = false;
		coin			  = 0;
	}
	
	// Getters
	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}
	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }
	bool IsStopUpdate() { return isStopUpdate; }
	int GetLevel() { return level; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetWidth();
	int GetHeight();
	int GetAnimationID();
	int GetOffsetY();

	// Setters
	void SetState(int state);
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	// Logic
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	// Collision related
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
};