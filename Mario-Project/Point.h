#include "GameObject.h"

#define POINT_BASE_ID 60000

#define POINT_STATE_BOUNCING 2
#define POINT_STATE_FALL 3
#define POINT_STATE_BOUNCING_END 4

#define POINT_BOUNCING_ACCEL 0.03f
#define POINT_Y_MAX_VEL 0.1f
#define POINT_BOUNCING_AMOUNT 32
#define POINT_VERTICAL_MOVE_OFFSET 8

class CPoint : public CGameObject
{
private:
	float ay;

	int point;
	
	int GetSpriteID() { return POINT_BASE_ID + point; }
public:
	CPoint(float x, float y);

	void virtual Render();
	void virtual Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void virtual GetBoundingBox(float& left, float& top, float& right, float& bottom) {}

	void SetPoint(int point) { this->point = point; }
	void SetState(int state);
};