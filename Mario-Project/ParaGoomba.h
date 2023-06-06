#pragma once

#include "Goomba.h"

#define STATE_WALKING_WITHOUT_WINGS 199
#define STATE_WALKING_WITH_WINGS 101
#define STATE_FLAPPING 102
#define STATE_FLYING 103
#define STATE_LANDING 104

class CParaGoomba : public CGoomba
{
	ULONGLONG flyTimer;
private:
	// Constants
	const int WIDTH_WITH_WINGS = 20;
	const int HEIGHT_WITH_WINGS = 16;
	const int WIDTH_WITHOUT_WINGS = 16;
	const int HEIGHT_WITHOUT_WINGS = 16;
	const int HEIGHT_FLAPPING = 24;

	// Accelerates, velocities
	const float ACC_Y_FLAPPING = -0.00125f;
	const float ACC_Y_LANDING = 0.005f;
	const float VEL_Y_MAX = 0.04f;

	// Time points
	const int POINT_FLAPPING_START = 500;
	const int POINT_FLAPPING_SWITCH_DIRECTION = 150;
	const int POINT_LANDING = 650;
	const int FLAPPING_MAX_COUNT = 8;
	const float VEL_X_FLY = -0.3f;

	const int ID_ANI_GOOMBA_PARA_WALKING_WITHOUT_WINGS = 5010;
	const int ID_ANI_GOOMBA_PARA_WALKING = 5011;
	const int ID_ANI_GOOMBA_PARA_WALKING_FLAPPING = 5012;
	const int ID_ANI_GOOMBA_PARA_FLY = 5013;
	const int ID_ANI_GOOMBA_PARA_DIE = 5101;
	
	int flappingCount = 0;

	// Getters
	bool HasWings() { return state >= STATE_WALKING_WITH_WINGS && state <= STATE_FLYING; }
	void GetChasingDirection();
	void GetDistanceWithPlayer(float& dx, float& dy);

	void ChasePlayer();
protected:
	// Getters
	virtual int GetAnimationID()
	{
		int aniId = ID_ANI_GOOMBA_PARA_WALKING;

		if (state == GOOMBA_STATE_DIE) {
			aniId = ID_ANI_GOOMBA_PARA_DIE;
		}
		else if (state == STATE_WALKING_WITHOUT_WINGS) {
			aniId = ID_ANI_GOOMBA_PARA_WALKING_WITHOUT_WINGS;
		}
		else if (state == STATE_FLAPPING) {
			aniId = ID_ANI_GOOMBA_PARA_WALKING_FLAPPING;
		}
		else if (state == STATE_FLYING) {
			aniId = ID_ANI_GOOMBA_PARA_FLY;
		}

		return aniId;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	// Logic
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
	// Constructors
	CParaGoomba(float x, float y) : CGoomba(x, y)
	{
		flyTimer = 0;
		SetState(STATE_WALKING_WITH_WINGS);
	}

	// Getters
	virtual int IsCollidable() { return state != GOOMBA_STATE_DIE; };
	virtual int IsBlocking() { return state != GOOMBA_STATE_DIE; }

	// Setters
	virtual void SetState(int state);

	// Collision related
	virtual void OnDamaged(LPCOLLISIONEVENT e);
};