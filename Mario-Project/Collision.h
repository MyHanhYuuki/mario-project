#pragma once

#include <Windows.h>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent 
{
	LPGAMEOBJECT src_obj;		// source object : the object from which to calculate collision
	LPGAMEOBJECT obj;			// the target object
	
	float t, nx, ny;

	float dx, dy;				// *RELATIVE* movement distance between this object and obj
	bool isDeleted;		

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, 
		LPGAMEOBJECT obj = NULL, LPGAMEOBJECT src_obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
		this->src_obj = src_obj;
		this->isDeleted = false;
	}

	int WasCollided() { return t >= 0.0f && t <= 1.0f; }
	bool IsVerticalCollision() { return ny != 0; }
	bool IsHorizontalCollision() { return nx != 0; }
	bool IsBothCollision() { return IsHorizontalCollision() && IsVerticalCollision(); }
	bool ShouldBeIgnored() { return isDeleted; }

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class CCollision
{
private:
	static CCollision* __instance;

	void ProcessBlockingCollision(LPGAMEOBJECT objSrc, DWORD dt, deque<LPCOLLISIONEVENT>& coEvents);
public: 
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPCOLLISIONEVENT SweptAABB(
		LPGAMEOBJECT objSrc, 
		DWORD dt,
		LPGAMEOBJECT objDest); 
	void Scan(
		LPGAMEOBJECT objSrc, 
		DWORD dt, 
		vector<LPGAMEOBJECT>* objDests, 
		deque<LPCOLLISIONEVENT>& coEvents);

	void Filter(
		LPGAMEOBJECT objSrc,
		deque<LPCOLLISIONEVENT>& coEvents,
		LPCOLLISIONEVENT &colX,
		LPCOLLISIONEVENT &colY, 
		bool shouldIgnoreNonBlocking,		
		bool shouldIncludeHorizontalCollision,
		bool shouldIncludeVerticalCollision);

	void Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	static CCollision* GetInstance();
};