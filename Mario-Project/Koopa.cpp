#include "Koopa.h"
#include "InvisiblePlatform.h"
#include "QuestionBrick.h"
#include "Goomba.h"

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPA_STATE_SHELL) {
		if (GetTickCount64() - onShellStart >= KOOPA_INSIDE_SHELL_TIMEOUT) {
			SetState(KOOPA_STATE_SHELL_TRANFORM);
		}
	}
	if (state == KOOPA_STATE_SHELL_TRANFORM) {
		if (GetTickCount64() - onShellStart >= KOOPA_INSIDE_SHELL_TIMEOUT) {
			SetState(KOOPA_STATE_WALKING);

			// Adjust height
			y += KOOPA_BBOX_HEIGHT_SHELL - height;
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		if (e->obj->CanGetThroughtOnCollision() == false) {
			vx = -vx;
			nx = -nx;
		}
	}

	if (dynamic_cast<CInvisiblePlatform*>(e->obj)) {
		OnCollisionWithInvisibleFlatform(e);
	} 

	// Collsion during shell move?
	if (state == KOOPA_STATE_SHELL_MOVING) {
		if (dynamic_cast<CQuestionBrick*>(e->obj)) {
			OnCollisionWithQuestionBrick(e);
		} else if (dynamic_cast<CGoomba*>(e->obj)) {
			OnCollisionWithGoomba(e);
		}
	}
}

int CKoopa::GetAnimationID()
{
	int aniId = 0;

	if (state == KOOPA_STATE_DIE)
	{
		aniId = ID_ANI_KOOPA_DIE;
	}
	else if (state == KOOPA_STATE_SHELL)
	{
		aniId = ID_ANI_KOOPA_SHELL;
	}
	else if (state == KOOPA_STATE_SHELL_MOVING)
	{
		aniId = ID_ANI_KOOPA_SHELL_MOVING;
	}
	else if (state == KOOPA_STATE_SHELL_TRANFORM)
	{
		aniId = ID_ANI_KOOPA_SHELL_TRANFORM;
	}
	else
	{
		aniId = nx == DIRECTION_LEFT ? ID_ANI_KOOPA_WALKING_LEFT : ID_ANI_KOOPA_WALKING_RIGHT;
	}

	return aniId;
}

void CKoopa::OnCollisionWithInvisibleFlatform(LPCOLLISIONEVENT e)
{
	if (e->ny == DIRECTION_TOP) {
		float xx, yy;
		e->obj->GetPosition(xx, yy);

		if (state == KOOPA_STATE_WALKING) {
			// Reach flatform left or right edge?
			if ((nx == DIRECTION_LEFT && x - xx <= 0) || (nx == DIRECTION_RIGHT && x + width >= xx + e->obj->GetWidth())) {
				vx = -vx;
				nx = -nx;
			}
		}
	}
}

void CKoopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	auto questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	if (questionBrick->IsCollidable() && questionBrick->GetState() != QUESTION_BRICK_STATE_EMPTY) {
		questionBrick->SetState(QUESTION_BRICK_STATE_BOUNCING);
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() != GOOMBA_STATE_DIE) {
		e->obj->SetState(GOOMBA_STATE_DIE_ON_COLLISION);
	}
}

void CKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CKoopa::Render()
{
	int aniId = GetAnimationID();

	// Calculate center point
	float centerX = x + width / 2;
	float centerY = y + height / 2;

	RenderBoundingBox(centerX, centerY);

	// TODO: Investigate magic number
	CAnimations::GetInstance()->Get(aniId)->Render(centerX, centerY + /*magic number*/2);
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case KOOPA_STATE_DIE:
			y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case KOOPA_STATE_WALKING:
			y += (height - KOOPA_BBOX_HEIGHT_SHELL) / 2;
			height = KOOPA_BBOX_HEIGHT;
			nx = -1;
			vx = -KOOPA_WALKING_SPEED;
			break;
		case KOOPA_STATE_SHELL:
			vx = 0;
			ax = 0;
			y += (height - KOOPA_BBOX_HEIGHT_SHELL) / 2;
			height = KOOPA_BBOX_HEIGHT_SHELL;
			onShellStart = GetTickCount64();
			break;
		case KOOPA_STATE_SHELL_MOVING:
			y += (height - KOOPA_BBOX_HEIGHT_SHELL) / 2;
			vx = -KOOPA_SHELL_MOVING_SPEED;
			height = KOOPA_BBOX_HEIGHT_SHELL;
			break;
		case KOOPA_STATE_SHELL_TRANFORM:
			onShellStart = GetTickCount64();
			break;
	}
}

void CKoopa::OnDamaged(LPCOLLISIONEVENT event, LPGAMEOBJECT sourceObject)
{
	// Walking -> turn into shell
	if (state == KOOPA_STATE_WALKING)
	{
		SetState(KOOPA_STATE_SHELL);
	}

	// Stand shell -> moving shell
	else if (state == KOOPA_STATE_SHELL) {
		SetState(KOOPA_STATE_SHELL_MOVING);
	}

	// Moving shell -> Stand shell
	else if (state == KOOPA_STATE_SHELL_MOVING) {
		SetState(KOOPA_STATE_SHELL);
	}
}