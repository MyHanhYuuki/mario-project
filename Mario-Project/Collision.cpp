#include "Collision.h"
#include "GameObject.h"

#include "debug.h"

#define BLOCK_PUSH_FACTOR 0.4f

CCollision* CCollision::__instance = NULL;

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB 
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	objSrc->GetSpeed(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	objDest->GetSpeed(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, deque<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		if (e->WasCollided()==1) {
			if (e->obj->IsBlocking()) {
				coEvents.push_front(e);
			} else {
				coEvents.push_back(e);
			}
		}
		else
			delete e;
	}
}

void CCollision::Filter( LPGAMEOBJECT objSrc,
	deque<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT &colX,
	LPCOLLISIONEVENT &colY,
	bool shouldIncludeHorizontalCollision = true,	// true: include events on X-axis, false: skip events on X 
	bool shouldIncludeVerticalCollision   = true,	// true: include events on Y-axis, false: skip events on Y
	bool shouldIgnoreNonBlocking          = true)	// true: only get block collisions, false: filter all collisions
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		// Ignore this event?
		if (coEvents[i]->ShouldBeIgnored()) continue;
		
		LPCOLLISIONEVENT c = coEvents[i];

		// Ignore non-blocking object collision?
		if (shouldIgnoreNonBlocking && c->obj->IsBlocking() == false) 
		{
			continue;
		}

		if (shouldIncludeHorizontalCollision) {
			
			// Find nearest collision time on x axis
			if (c->IsHorizontalCollision() && c->t < min_tx) {
				min_tx = c->t;
				min_ix = i;
			}
		}

		if (shouldIncludeVerticalCollision) {
			
			// Find nearest collision time on y axis
			if (c->IsVerticalCollision() && c->t < min_ty) {

				// Ignore collision on top?
				if (c->obj->GetBlockDirection() == BLOCK_TOP && c->ny > 0) {
					continue;
				}

				min_ty = c->t;
				min_iy = i;
			}
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Simple/Sample collision framework 
*  NOTE: Student might need to improve this based on game logic 
*/
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	deque<LPCOLLISIONEVENT> coEvents;

	coEvents.clear();

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	// No collision detected
	if (coEvents.empty()) {
		objSrc->OnNoCollision(dt);
	} else {
		 // Because of order of event list is {blockingEvents, non-blockingEvents}
		 // firstly process all non-blocking collision and blocking ones
		 do {
		 	// Get element at the end
		 	LPCOLLISIONEVENT e = coEvents.back();

		 	// Ignore collision?
		 	if (e->ShouldBeIgnored() == false && e->obj->IsDeleted() == false) {

		 		// Process non-blocking collision
		 		if (e->obj->IsBlocking() == false){
		 			if (e->obj->IsCollidable()) objSrc->OnCollisionWith(e);
		 		}
				
		 		// Process blocking collision
		 		else {
		 			ProcessBlockingCollision(objSrc, dt, coEvents);

		 			// When blocking event is processed that means process is done
		 			coEvents.clear();
					coEvents.shrink_to_fit();
		 			break;
		 		}
		 	}

		 	// Release processed event
		 	coEvents.pop_back();
		 	delete e;

		 } while(!coEvents.empty());
	}
}

 void CCollision::ProcessBlockingCollision(LPGAMEOBJECT objSrc, DWORD dt, deque<LPCOLLISIONEVENT>& coEvents)
 {
 	LPCOLLISIONEVENT colX = NULL;
 	LPCOLLISIONEVENT colY = NULL;

 	float x, y, vx, vy, dx, dy;
 	objSrc->GetPosition(x, y);
 	objSrc->GetSpeed(vx, vy);
 	dx = vx * dt;
 	dy = vy * dt;

 	// TODO: refactor
 	// colX = coEvents.back();
 	// colY = coEvents[coEvents.size() - 2];
 	Filter(objSrc, coEvents, colX, colY);

 	if (colX != NULL && colY != NULL) 
 	{
 		if (colY->t < colX->t)	// was collision on Y first ?
 		{
 			// Block source object and push them away
			if (colY->src_obj->IsMoving()) {
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
			}
			else {
				y += dy;
			}

 			objSrc->SetPosition(x, y);

 			objSrc->OnCollisionWith(colY);

 			//
 			// see if after correction on Y, is there still a collision on X ? 
 			//
 			LPCOLLISIONEVENT colX_other = NULL;

 			//
 			// check again if there is true collision on X 
 			//
 			colX->isDeleted = true;		// remove current collision event on X

 			// replace with a new collision event using corrected location 
 			coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

 			// re-filter on X only
 			Filter(objSrc, coEvents, colX_other, colY, true, false);

 			if (colX_other != NULL)
 			{
				if (colX->src_obj->IsMoving()) {
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
				}
				else {
					x += dx;
				}
 				objSrc->OnCollisionWith(colX_other);
 			}
 			else
 			{
 				x += dx;
 			}
 		}
 		else // collision on X first
 		{
 			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
			if (colX->src_obj->IsMoving()) {
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
			}
			else {
				x += dx;
			}
 			objSrc->SetPosition(x, y);

 			objSrc->OnCollisionWith(colX);

 			//
 			// see if after correction on X, is there still a collision on Y ? 
 			//
 			LPCOLLISIONEVENT colY_other = NULL;

 			//
 			// check again if there is true collision on Y
 			//
 			colY->isDeleted = true;		// remove current collision event on Y

 			// replace with a new collision event using corrected location 
 			coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

 			// re-filter on Y only
 			Filter(objSrc, coEvents, colX, colY_other, false, true);

 			if (colY_other != NULL)
 			{
				if (colY->src_obj->IsMoving()) {
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
				}
				else {
					y += dy;
				}
 				objSrc->OnCollisionWith(colY_other);
 			}
 			else
 			{
 				y += dy;
 			}
 		}
 	}
 	else if (colX != NULL)
 	{
 		y += dy;
		if (colX->src_obj->IsMoving()) {
			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
		}
		else {
			x += dx;
		}

 		objSrc->OnCollisionWith(colX);
 	}
 	else if (colY != NULL)
 	{
 		x += dx;
		if (colY->src_obj->IsMoving()) {
			y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
		}
		else {
			y += dy;
		}

 		objSrc->OnCollisionWith(colY);
 	}
 	else // both colX & colY are NULL 
 	{
 		x += dx;
 		y += dy;
 	}

 	objSrc->SetPosition(x, y);
 }