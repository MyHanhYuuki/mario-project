#pragma once

#include "GameObject.h"

#define	VENUS_FIRE_TRAP_WIDTH 16
#define VENUS_FIRE_TRAP_BBOX_WIDTH 16
#define VENUS_FIRE_TRAP_BBOX_HEIGHT 32

#define VENUS_FIRE_TRAP_STATE_BOUNCING 1
#define VENUS_FIRE_TRAP_STATE_FALL 2
#define VENUS_FIRE_TRAP_STATE_STAND 3
#define VENUS_FIRE_TRAP_STATE_FALL_STAND 4
#define VENUS_FIRE_TRAP_STATE_FIRED 5

#define VENUS_FIRE_TRAP_BOUNCING_ACCEL 0.005f
#define VENUS_FIRE_TRAP_Y_MAX_VEL 0.03f
#define VENUS_FIRE_TRAP_STAND_AMOUNT 2000
#define VENUS_FIRE_TRAP_FIRE_TRIGGER_AMOUNT 1000
#define VENUS_FIRE_TRAP_FIRABLE_MIN_DISTANCE 32
#define VENUS_FIRE_TRAP_FIRABLE_MAX_DISTANCE 160
#define VENUS_FIRE_TRAP_FALL_OFFSET VENUS_FIRE_TRAP_BBOX_HEIGHT + 8

class CVenusFireTrap : public CGameObject
{
private:
	ULONGLONG standStart;

	int GetSpriteID();
	void GetDistanceWithPlayer(float &dx, float &dy);
	bool IsFirable();

	void CalculateFaceDirection();
public:
    CVenusFireTrap(float x, float y) : CGameObject(x, y)
	{
		this->blockDirection = BLOCK_NONE;
		this->y += VENUS_FIRE_TRAP_BBOX_HEIGHT;
		standStart = 0;

		SetState(VENUS_FIRE_TRAP_STATE_BOUNCING);
	}
	CVenusFireTrap(string name, float x, float y) : CVenusFireTrap(x, y) {
		// Extract property from name
		if ((int)name.rfind("Green") >= 0) {

		} else {
			width = VENUS_FIRE_TRAP_BBOX_WIDTH;
			height = VENUS_FIRE_TRAP_BBOX_HEIGHT;
		}
	}

    void SetState(int state);
	
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsBlocking() { return 0; }
};

