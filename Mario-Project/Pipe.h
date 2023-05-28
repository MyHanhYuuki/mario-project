#pragma once

#include "GameObject.h"

#define ID_ANI_BRICK 10000
#define PIPE_BASE_WIDTH 32
#define PIPE_BASE_HEIGHT 32
#define PIPE_VERTICAL_HALF_WIDTH 32
#define PIPE_VERTICAL_HALF_HEIGHT 16
#define PIPE_HORIZONTAL_HALF_WIDTH 16
#define PIPE_HORIZONTAL_HALF_HEIGHT 32

class CPipe : public CGameObject
{
private:
	bool isVertial;
	int countOfHalfPart;
public:
	CPipe(float x, float y, int width, int height): CGameObject(x, y, width, height)
	{
		this->isVertial = true;
		this->countOfHalfPart = 0;
	}
	CPipe(string name, float x, float y, int width, int height) : CPipe(x, y, width, height) {
		this->name = name;

		// Extract property from name
		this->isVertial = ((int)name.rfind("Vertical")) >= 0;
		if (isVertial) {
			countOfHalfPart = (height - PIPE_BASE_HEIGHT) / PIPE_VERTICAL_HALF_HEIGHT;
		} else {
			countOfHalfPart = (width - PIPE_BASE_WIDTH) / PIPE_HORIZONTAL_HALF_WIDTH;
		}
	}

	void virtual SetState(int state);

	void virtual Render();
	void virtual Update(DWORD dt) {}
	void virtual GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
