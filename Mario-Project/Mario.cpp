#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "InvisiblePlatform.h"
#include "QuestionBrick.h"
#include "Mushroom.h"
#include "LifeMushroom.h"
#include "Leaf.h"
#include "VenusFireTrap.h"
#include "FireBall.h"
#include "Koopa.h"
#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// Kicking? then become
	if (GetTickCount64() - kickStart > KICH_TIMEOUT) {
		kickStart = 0;
		isKicking = false;
	}

	// Holding?
	if (isHolding) {
		// Update holding item position
		if (CGame::GetInstance()->IsKeyDown(DIK_A)) {
			// Location = center point + offset (= direction indicator value multiply by hold object width)
			auto holdPosX = (x - GetWidth() / 2) + (nx * (holdItem->GetWidth() + HOLD_X_OFFSET));
			auto holdPosY = y - holdItem->GetHeight() / 2 + (level == MARIO_LEVEL_SMALL ? HOLD_X_OFFSET : 0);

			holdItem->SetPosition(holdPosX, holdPosY);
		}

		// Kick it away
		else {
			isHolding = false;

			holdItem->SetHorizontalDirection(nx);
			holdItem->SetState(KOOPA_STATE_SHELL_MOVING);
		}
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"coin: %d", coin);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->GetBlockDirection() == BLOCK_ALL) {
		if (e->ny != 0) {
			vy = 0;
			if (e->ny < 0) isOnPlatform = true;
		}
		else if (e->nx != 0) {
			vx = 0;
		}
	} else if (e->obj->GetBlockDirection() == BLOCK_TOP) {
		if (e->ny < 0) {
			isOnPlatform = true;
			vy = 0;
		}
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
		OnCollisionWithQuestionBrick(e);
	}
	else if (dynamic_cast<CLifeMushroom*>(e->obj)) {
		OnCollisionWithLifeMushroom(e);
	}
	else if (dynamic_cast<CMushroom*>(e->obj)) {
		OnCollisionWithMushroom(e);
	}
	else if (dynamic_cast<CLeaf*>(e->obj)) {
		OnCollisionWithLeaf(e);
	}
	else if (dynamic_cast<CVenusFireTrap*>(e->obj)) {
		OnCollisionWithVenueFireTrap(e);
	}
	else if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->GetState() != GOOMBA_STATE_DIE)
	{
		// jump on top >> kill Goomba and deflect a bit 
		if (e->ny < 0)
		{
			goomba->OnDamaged(e);
			vy = -MARIO_JUMP_DEFLECT_SPEED;

			float xx, yy;
			goomba->GetPosition(xx, yy);

			// Emit event
			FireGainPointEvent(xx, yy - GOOMBA_BBOX_HEIGHT_DIE);
		}

		// hit by Goomba
		else
		{
			if (untouchable == 0)
			{
				OnCollisionWithEnemy(e);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	coin++;
	e->obj->SetState(COIN_STATE_COLLECTED);
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e) {
	if (e->obj->GetState() == QUESTION_BRICK_STATE_NEW) {
		if (e->ny > 0) {
			e->obj->SetState(QUESTION_BRICK_STATE_BOUNCING);
		}
	}
}

void CMario::OnCollisionWithLifeMushroom(LPCOLLISIONEVENT e)
{
	e->obj->Delete();

	float xx, yy;
	e->obj->GetPosition(xx, yy);

	// Emit event
	FireGainPointEvent(xx, yy - MUSHROOM_BBOX_HEIGHT, 8001);
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	SetLevel(MARIO_LEVEL_BIG);

	float xx, yy;
	e->obj->GetPosition(xx, yy);

	// Emit event
	FireGainPointEvent(xx, yy - MUSHROOM_BBOX_HEIGHT, 1000);
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	SetLevel(MARIO_LEVEL_TANUKI);

	float xx, yy;
	e->obj->GetPosition(xx, yy);

	// Emit event
	FireGainPointEvent(xx, yy - MUSHROOM_BBOX_HEIGHT, 1000);
}

void CMario::OnCollisionWithVenueFireTrap(LPCOLLISIONEVENT e)
{
	OnCollisionWithEnemy(e);
}

void CMario::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (untouchable == 0) {
		if (level > MARIO_LEVEL_SMALL)
		{
			level -= 1;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithFireball(LPCOLLISIONEVENT e)
{
	auto fireball = dynamic_cast<CFireBall*>(e->obj);
	if (fireball->IsFromPlayer()) return;

	OnCollisionWithEnemy(e);
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// jump on top
	if (e->ny < 0)
	{
		// Turn it into a shell
		koopa->OnDamaged(e, this);

		// Deflect
		vy = -MARIO_JUMP_DEFLECT_SPEED;

		// Emit event
		FireGainPointEvent(koopa->GetPosX(), koopa->GetPosY() - koopa->GetHeight());
	}
	else // hit by koopa
	{
		if (koopa->GetState() == KOOPA_STATE_WALKING || koopa->GetState() == KOOPA_STATE_SHELL_MOVING)
		{
			if (untouchable == 0)
			{
				OnCollisionWithEnemy(e);
			}
		} else {
			// hold shell?
			if (CGame::GetInstance()->IsKeyDown(DIK_A)) {
				isHolding = true;
				holdItem  = koopa;

				koopa->SetBeingHold(true);
			}
			
			// Kich shell?
			else {
				SetState(MARIO_STATE_KICK);
				koopa->OnDamaged(e, this);
			}
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (isKicking) {
					aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isKicking) {
					aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (isKicking) {
					aniId = ID_ANI_MARIO_KICK_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isKicking) {
					aniId = ID_ANI_MARIO_KICK_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniIdTanuki()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_TANUKI_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_TANUKI_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_TANUKI_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_TANUKI_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_TANUKI_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_TANUKI_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_TANUKI_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_TANUKI_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (isKicking) {
					aniId = ID_ANI_MARIO_TANUKI_KICK_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_TANUKI_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TANUKI_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TANUKI_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isKicking) {
					aniId = ID_ANI_MARIO_TANUKI_KICK_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_TANUKI_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TANUKI_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TANUKI_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_TANUKI_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAnimationID()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_TANUKI)
		aniId = GetAniIdTanuki();

	return aniId;
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_KICK:
		kickStart = GetTickCount64();
		isKicking = true;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (IsHasAnimation()) {
		LPSPRITE curSprite = GetCurrentAnimation()->GetCurrentAnimationFrame()->GetSprite();
		width = curSprite->GetWidth();
		height = curSprite->GetHeight();
	}
	else {
		width = GetWidth();
		height = GetHeight();
	}

	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

int CMario::GetWidth()
{
	int width = MARIO_SMALL_BBOX_WIDTH;

	if (level == MARIO_LEVEL_BIG)
	{
		width = isSitting ? MARIO_BIG_SITTING_BBOX_WIDTH : MARIO_BIG_BBOX_WIDTH;
	}
	else if (level == MARIO_LEVEL_TANUKI)
	{
		width = isSitting ? MARIO_TANUKI_SITTING_BBOX_WIDTH : MARIO_BIG_BBOX_WIDTH;
	}

	return width;
}

int CMario::GetHeight()
{
	int height = MARIO_SMALL_BBOX_HEIGHT;

	if (level == MARIO_LEVEL_BIG)
	{
		height = isSitting ? MARIO_BIG_SITTING_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TANUKI)
	{
		height = isSitting ? MARIO_TANUKI_SITTING_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
	}

	return height;
}

int CMario::GetOffsetY() {
	if (isSitting) {
		return MARIO_BIG_OFFSET_Y_SITTING;
	}
	else {
		return MARIO_BIG_OFFSET_Y_IDLE;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2.0;
	}
	level = l;
}

void CMario::FireGainPointEvent(float x, float y, int point) {
	// Emit gain point event
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->FireGainPointEvent(
																	EVENT_GAIN_POINT
																	, x
																	, y
																	, point
																);
}
