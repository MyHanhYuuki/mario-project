#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"
#include "AssetIDs.h"
#include "const.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject
{
protected:
	// Object name
	string name;

	float originalX;
	float originalY;
	float x;
	float y;

	int width;
	int height;

	int state;
	bool isDeleted;

	//===========================================
	// Collision related property
	//===========================================
	float vx;
	float vy;

	float ax;
	float ay;

	int nx;	 
	int ny;

	int blockDirection;
public:
	// Constructor
	CGameObject()
	{
		originalX = originalY = x = y = 0;
		width = height = 0;
		vx = vy = 0;
		ax = ay = 0;
		nx = ny = 1;
		state = -1;
		isDeleted = false;
		blockDirection = BLOCK_ALL;
	}
	CGameObject(float x, float y) : CGameObject() {
		this->originalX = this->x = x;
		this->originalY = this->y = y;
	}
	CGameObject(float x, float y, int width, int height) : CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
	}
	CGameObject(string name, float x, float y) : CGameObject(x, y) {
		this->name = name;
	}
	CGameObject(string name, float x, float y, int width, int height) : CGameObject(name, x, y) {
		this->width = width;
		this->height = height;
	}

	// Getters
	int GetState() { return this->state; }
	bool IsDeleted() { return isDeleted; }
	void GetCenterPoint(float &top, float &left) {
		left = x + width / 2;
		top = y + height / 2;
	}
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	float GetPosX() { return x; }
	float GetPosY() { return y; }
	virtual int GetAnimationID() { return -1; }
	virtual D3DXMATRIX* GetCustomTranformationMatrix() { return NULL; }

	// Setters
	void SetName(string name) { this->name = name; }
	virtual void SetState(int state) { this->state = state; }

	// Logic handler
	virtual void Init() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render()
	{
		RenderBoundingBox();

		auto aniID = GetAnimationID();
		auto matCustomTranformation = GetCustomTranformationMatrix();

		CAnimations::GetInstance()->Get(aniID)->Render(x, y, matCustomTranformation);
	}

	virtual void Delete() { isDeleted = true; }

	// Deconstructor
	~CGameObject();

	//===========================================
	// Collision related function
	//===========================================
	
	// Getters
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	virtual int IsCollidable() { return 1; };
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return blockDirection != BLOCK_NONE; }
	int GetBlockDirection() { return this->blockDirection; }
	bool IsMoving() { return vx != 0 || vy != 0; }

	// Setters
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetBlockDirection(int blockDirection) { this->blockDirection = blockDirection; }

	// Logic Handler
	void RenderBoundingBox();
	void RenderBoundingBox(float left, float top);

	// Event Handler
	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};
	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
};
