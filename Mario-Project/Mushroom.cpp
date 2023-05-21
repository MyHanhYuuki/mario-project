#include "Mushroom.h"

#include "Coin.h"
#include "Utils.h"

CMushroom::CMushroom(float x, float y) : CGameObject(x, y) {
	blockDirection = BLOCK_NONE;
	ay = 0;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vy = Clamp(vy, -MUSHROOM_Y_MAX_VEL, MUSHROOM_Y_MAX_VEL);

	if (state == MUSHROOM_STATE_BOUNCING) {
		if (originalY - y >= MUSHROOM_BOUNCING_AMOUNT) {
			SetState(MUSHROOM_STATE_FALL);
		}

        OnNoCollision(dt);
	} else {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
};

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	// if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0 )
	{
		if (state == MUSHROOM_STATE_FALL) {
            SetState(MUSHROOM_STATE_BOUNCING_MOVE);
        }
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
};

void CMushroom::SetState(int state)
{ 
	this->state = state;

	switch (state) {
		case MUSHROOM_STATE_BOUNCING:
			ay = -MUSHROOM_BOUNCING_ACCEL;
			break;
		case MUSHROOM_STATE_FALL:
			ay = MUSHROOM_BOUNCING_ACCEL;
			vy = 0;
			break;
		case MUSHROOM_STATE_BOUNCING_END:
			ay = 0;
			vy = 0;
			y = originalY;
			isDeleted = true;
			break;
        case MUSHROOM_STATE_BOUNCING_MOVE:
            float playerX, playerY;
            ((CPlayScene*) CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(playerX, playerY);

            vx = x - playerX >= 0 ? MUSHROOM_X_VEL : -MUSHROOM_X_VEL;
	}
}

void CMushroom::Render()
{
    CSprites::GetInstance()->Get(ID_SPRITE_MUSROOM)->Draw(x, y);

	RenderBoundingBox();
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}