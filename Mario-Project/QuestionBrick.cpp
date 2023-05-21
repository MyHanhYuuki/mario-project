#include "Utils.h"
#include "QuestionBrick.h"
#include "Mario.h"
#include "Coin.h"
#include "Mushroom.h"

CQuestionBrick::CQuestionBrick(float x, float y): CGameObject(x, y)
{
	Init();
}

void CQuestionBrick::Init() {
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
		case QUESTION_BRICK_STATE_BOUNCING:
			ay = -QUESTION_BRICK_BOUNCING_ACCEL;
			break;
		case QUESTION_BRICK_STATE_BOUNCING_END:
			ay = QUESTION_BRICK_BOUNCING_ACCEL;
			break;
		case QUESTION_BRICK_STATE_EMPTY:
			ay = 0;
			vy = 0;
			break;
	}
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {
	vy += ay * dt;
	vy = Clamp(vy, -QUESTION_Y_MAX_VEL, QUESTION_Y_MAX_VEL);

	if (state == QUESTION_BRICK_STATE_BOUNCING) {
		if (originalY - y >= QUESTION_BRICK_BOUNCING_AMOUNT) {
			// Create inner item (vd: Coin, Mushroom)
			// Check inner item (default item is coin)
			LPGAMEOBJECT innerItem = nullptr;

			// Extract inner item by name
			if ((int)name.rfind("LifeMushroom") > 0) {

			}
			else if ((int)name.rfind("Levelup") >= 0) {
				innerItem = new CMushroom(originalX, originalY-QUESTION_BRICK_BOUNCING_AMOUNT/2);
				innerItem->SetState(MUSHROOM_STATE_BOUNCING);
			}
			else {
				innerItem = new CCoin(originalX, originalY - QUESTION_BRICK_BOUNCING_AMOUNT);
				innerItem->SetState(COIN_STATE_BOUNCING);
			}

			// Add item into game world before its container
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddGameObjectBefore(this, innerItem);

			SetState(QUESTION_BRICK_STATE_BOUNCING_END);
		}
	} else if (state == QUESTION_BRICK_STATE_BOUNCING_END) {
		if (y - originalY >= 0) {
			// Return to original position
			y = originalY;

			SetState(QUESTION_BRICK_STATE_EMPTY);
		}
	}

	y += vy * dt;
}