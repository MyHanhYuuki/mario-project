#include "VenusFireTrap.h"
#include "Utils.h"
#include "Playscene.h"
#include "FireBall.h"

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == VENUS_FIRE_TRAP_STATE_BOUNCING) {
		if (originalY - y >= 0) {
			SetState(VENUS_FIRE_TRAP_STATE_STAND);
		}
	}

	else if (state == VENUS_FIRE_TRAP_STATE_STAND) {
		if (GetTickCount64() - standStart >= VENUS_FIRE_TRAP_FIRE_TRIGGER_AMOUNT) {
			if (IsFirable()) {
				// Sould double x vel ?
				float dx, dy;
				GetDistanceWithPlayer(dx, dy);
				auto souldDoubleXVel = abs(dx/dy) > 1;
				
				auto fireball = new CFireBall(x, y);
				fireball->SetSpeed(nx*FIREBALL_X_MAX_VEL* (souldDoubleXVel ? 2 : 1), ny*FIREBALL_Y_MAX_VEL);

				// Add item into game world before its container
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddGameObjectBefore(this, fireball);
			}

			SetState(VENUS_FIRE_TRAP_STATE_FIRED);
		}
		
	}

	else if (state == VENUS_FIRE_TRAP_STATE_FIRED) {
		if (GetTickCount64() - standStart >= VENUS_FIRE_TRAP_STAND_AMOUNT) {
			SetState(VENUS_FIRE_TRAP_STATE_FALL);
		}
	}

	else if (state == VENUS_FIRE_TRAP_STATE_FALL) {
		if (y - originalY >= VENUS_FIRE_TRAP_FALL_OFFSET) {
			SetState(VENUS_FIRE_TRAP_STATE_FALL_STAND);
		}
	}

	else if (state == VENUS_FIRE_TRAP_STATE_FALL_STAND) {
		// Raise only when is it firable
		if (GetTickCount64() - standStart >= VENUS_FIRE_TRAP_STAND_AMOUNT) {
			float dx, dy;
			GetDistanceWithPlayer(dx, dy);

			if (abs(dx) >= VENUS_FIRE_TRAP_FIRABLE_MIN_DISTANCE) {
				SetState(VENUS_FIRE_TRAP_STATE_BOUNCING);
			}
		}
	}


	vy = Clamp(vy, -VENUS_FIRE_TRAP_Y_MAX_VEL, VENUS_FIRE_TRAP_Y_MAX_VEL);
	
	x += vx * dt;
	y += vy * dt;
};

void CVenusFireTrap::GetDistanceWithPlayer(float &dx, float &dy) {
	// Calculate base on player position
	float playerX, playerY;
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(playerX, playerY);

	dx = (x + VENUS_FIRE_TRAP_BBOX_WIDTH/2) - playerX;
	dy = y - playerY;
}

void CVenusFireTrap::CalculateFaceDirection()
{
	// Calculate base on player position
	float dx, dy;
	GetDistanceWithPlayer(dx, dy);

	nx = dx >= 0 ? -1 : 1;
	ny = dy >= 0 ? -1 : 1;
}

int CVenusFireTrap::GetSpriteID()
{
	CalculateFaceDirection();

    if (nx == -1) {
		return ny == -1 ? ID_VENUS_FIRE_TRAP_LEFT_TOP : ID_VENUS_FIRE_TRAP_LEFT_BOTTOM;
	} else {
		return ny == -1 ? ID_VENUS_FIRE_TRAP_RIGHT_TOP : ID_VENUS_FIRE_TRAP_RIGHT_BOTTOM;
	}
}

void CVenusFireTrap::SetState(int state)
{ 
	this->state = state;

	switch (state) {
		case VENUS_FIRE_TRAP_STATE_BOUNCING:
			ay = -VENUS_FIRE_TRAP_BOUNCING_ACCEL;
			break;
		case VENUS_FIRE_TRAP_STATE_FALL:
			ay = VENUS_FIRE_TRAP_BOUNCING_ACCEL;
			vy = 0;
			break;
		case VENUS_FIRE_TRAP_STATE_STAND:
			ay = 0;
			vy = 0;
			standStart = GetTickCount64();
			break;
		case VENUS_FIRE_TRAP_STATE_FALL_STAND:
			ay = 0;
			vy = 0;
			standStart = GetTickCount64();
			break;
	}
}

void CVenusFireTrap::Render()
{
	CSprites::GetInstance()->Get(GetSpriteID())->Draw(x, y);

	RenderBoundingBox();
}

void CVenusFireTrap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - height / 2;
	r = l + width;
	b = t + height;
}

bool CVenusFireTrap::IsFirable() {
	float dx, dy;
	GetDistanceWithPlayer(dx, dy);

	// Raise only when is it firable
	if (abs(dx) <= VENUS_FIRE_TRAP_FIRABLE_MAX_DISTANCE) {
		return true;
	}

	return false;
}