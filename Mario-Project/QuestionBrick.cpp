#include "QuestionBrick.h"
#include "Mario.h"

CQuestionBrick::CQuestionBrick(float x, float y): CGameObject(x, y)
{
	Init();
}

void CQuestionBrick::Init() {
	bounceState = 0;
	startBounceTime = 0;
	bounceDelta = 0.0f;
	state = QUESTION_BRICK_STATE_NEW;

	ax = 0;
	ay = 0;
}

void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	auto aniId = state == QUESTION_BRICK_STATE_EMPTY ? ID_ANI_QUESTION_BRICK + 1 : ID_ANI_QUESTION_BRICK;
	animations->Get(aniId)->Render(x, y);
	
	RenderBoundingBox();
}

void CQuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::SetState(int state) {
	this->state = state;

	switch (state) {
		case QUESTION_BRICK_STATE_BOUCING:
			ay = -0.001;
			break;
		case QUESTION_BRICK_STATE_EMPTY:
			ay = 0;
			vy = 0;
			break;
	}
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {
	if (state == QUESTION_BRICK_STATE_BOUCING) {
		if (originalY - y >= 4) {
			ay = 0.001;
		} else if (y - originalY >= 0) {
			y = originalY;
			SetState(QUESTION_BRICK_STATE_EMPTY);
		}
	}

	vy += ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBrick::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

void CQuestionBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == QUESTION_BRICK_STATE_NEW) {
		if (dynamic_cast<CMario*>(e->obj)) {
			if (e->ny < 0) {
				SetState(QUESTION_BRICK_STATE_BOUCING);
			}
		}
	}
}