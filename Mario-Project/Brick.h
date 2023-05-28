#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject {
public:
	// Constructor
	CBrick(float x, float y) : CGameObject(x, y, BRICK_BBOX_WIDTH, BRICK_BBOX_HEIGHT) {}
	
	// Setters
	void virtual SetState(int state);

	// Logic function
	void virtual Render();
	void virtual Update(DWORD dt) {}
	void virtual GetBoundingBox(float& left, float& top, float& right, float& bottom);
};