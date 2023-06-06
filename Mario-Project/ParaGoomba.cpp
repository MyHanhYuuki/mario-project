#include "ParaGoomba.h"
#include "PlayScene.h"
#include "Utils.h"

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == STATE_WALKING_WITH_WINGS) {
		// Chase only when is on ground
		if (IsMoving() && vy == 0) {
			ChasePlayer();
		}

		// Can flap wings?
		if (GetTickCount64() - flyTimer >= POINT_FLAPPING_START) {
			SetState(STATE_FLAPPING);
		}
	}

	else if (state == STATE_FLAPPING) {
		// Flap enough then fly
		if (flappingCount == FLAPPING_MAX_COUNT) {
			SetState(STATE_FLYING);
		}

		// Otherwise continue flapping
		else if (vy <= 0) {
			if (GetTickCount64() - flyTimer >= POINT_FLAPPING_SWITCH_DIRECTION) {
				flyTimer = GetTickCount64();
				ay = -ay;
				flappingCount++;
			}
		}
	}

	// Fly enough then landing
	else if (state == STATE_FLYING) {
		// Note: collision with blocking during flying cause vy = 0
		if (vy <= 0) {
			if (GetTickCount64() - flyTimer >= POINT_LANDING) {
				SetState(STATE_LANDING);
			}
		}
	}

	// Is Landed then continue move
	else if (state == STATE_LANDING) {
		if (vy == 0) {
			SetState(STATE_WALKING_WITH_WINGS);
		}
	}

	CGoomba::Update(dt, coObjects);

	// Clamp velocity
	vy = Clamp(vy, -VEL_Y_MAX, VEL_Y_MAX);
}

void CParaGoomba::ChasePlayer()
{
	// Get chasing direction
	GetChasingDirection();

	// Change moving direction
	vx = (vx * nx) > 0 ? vx : -vx;
}

void CParaGoomba::GetChasingDirection()
{
	// Calculate base on player position
	float dx, dy;
	GetDistanceWithPlayer(dx, dy);

	nx = dx >= 0 ? DIRECTION_LEFT : DIRECTION_RIGHT;
}

void CParaGoomba::GetDistanceWithPlayer(float& dx, float& dy) {
	// Calculate base on player position
	float playerX, playerY;
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(playerX, playerY);

	dx = (x + width / 2.0f) - playerX;
	dy = y - playerY;
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2.0f;
	top = y - height / 2.0f;
	right = left + width;
	bottom = top + height;
}

void CParaGoomba::SetState(int state)
{
	CGoomba::SetState(state);

	switch (state)
	{
	case STATE_WALKING_WITH_WINGS:
		width = WIDTH_WITH_WINGS;
		height = HEIGHT_WITH_WINGS;
		vy = GOOMBA_GRAVITY;
		vx = nx * GOOMBA_WALKING_SPEED;
		flyTimer = GetTickCount64();
		break;
	case STATE_WALKING_WITHOUT_WINGS:
		y += -(height - WIDTH_WITHOUT_WINGS) / 2.0f;
		width = WIDTH_WITHOUT_WINGS;
		height = HEIGHT_WITHOUT_WINGS;
		ay = ACC_Y_LANDING;
		vy = GOOMBA_GRAVITY;
		vx = nx * GOOMBA_WALKING_SPEED;
		break;
	case STATE_FLAPPING:
		y += (height - HEIGHT_FLAPPING)/2;
		height = HEIGHT_FLAPPING;
		ay = ACC_Y_FLAPPING;
		vx = nx * GOOMBA_WALKING_SPEED/2;
		flappingCount = 0;
		break;
	case STATE_FLYING:
		vy = VEL_X_FLY;
		ay = 0;
		ax = 0;
		flappingCount = 0;
		flyTimer = GetTickCount64();
		break;
	case STATE_LANDING:
		ay = ACC_Y_LANDING;
		flyTimer = 0;
		break;
	}

}

void CParaGoomba::OnDamaged(LPCOLLISIONEVENT e)
{
	if (HasWings()) {
		SetState(STATE_WALKING_WITHOUT_WINGS);
	}
	else if (state == STATE_WALKING_WITHOUT_WINGS) {
		SetState(GOOMBA_STATE_DIE);
	}
}