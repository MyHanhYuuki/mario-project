#include "QuestionBrick.h"
#include "PlayScene.h"

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == QUESTION_BRICK_STATE_BOUNCING_UP)
	{
		vy += ay * dt;
		y += vy * dt;
	}

	if (item != NULL) {
		item->Update(dt, coObjects);
	}

	if (y < default_pos_y)
	{
		this->SetState(QUESTION_BRICK_STATE_IDLE_NO_ITEM);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBrick::Render()
{
	if (item != NULL) {
		item->Render();
	}

	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	
	float width = right - left;
	float height = bottom - top;
	int aniId = this->GetAniId();
	CAnimations::GetInstance()->Get(aniId)->Render(x + width / 2, y + height / 2);

	this->RenderBoundingBox();
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + QUESTION_BRICK_BBOX_WIDTH;
	b = t + QUESTION_BRICK_BBOX_HEIGHT;
}

int CQuestionBrick::GetAniId()
{
	int id = -1;
	if (state != QUESTION_BRICK_STATE_IDLE_NO_ITEM)
		id = ID_ANI_QUESTION_BRICK_QUESTION_MARK_MOVING;
	else
		id = ID_ANI_QUESTION_BRICK_NO_QUESTION_MARK_MOVING;

	return id;
}

void CQuestionBrick::SetState(int state)
{
	switch (state)
	{
		case QUESTION_BRICK_STATE_IDLE_HAVE_ITEM:
			vy = 0;
			ay = 0;
			break;
		case QUESTION_BRICK_STATE_IDLE_NO_ITEM:
			vy = 0;
			ay = 0;
			y = default_pos_y;
			break;
		case QUESTION_BRICK_STATE_BOUNCING_UP:
			vy = -QUESTION_BRICK_BOUNCING_UP_SPEED;
			ay = QUESTION_BRICK_GRAVITY;
			break;
	}

	this->state = state;
}

void CQuestionBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) {
		if (state == QUESTION_BRICK_STATE_IDLE_HAVE_ITEM && e->ny < 0)
		{
			this->SetState(QUESTION_BRICK_STATE_BOUNCING_UP);

			this->item = new CMushroom(x, y, this);
			this->item->SetState(MUSHROOM_STATE_RISING);
		}
	}
};

void CQuestionBrick::OnNoCollision(DWORD dt) {};