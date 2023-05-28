#include "Pipe.h"
#include "Textures.h"

void CPipe::Render()
{
	float t, l, b, r;
	GetBoundingBox(l, t, r, b);
	
	int loopCount = countOfHalfPart;
	// Draw vertical pipe
	if (isVertial) {
		// Draw base pipe
		CSprites::GetInstance()->Get(ID_PIPE_VETICAL_FULL)->Draw(x + width/2, t + PIPE_BASE_HEIGHT / 2);

		// Draw bottom pipe
		while(loopCount > 0) {
			// Calculate dynamic part height and offset
			auto top = t + PIPE_BASE_HEIGHT + PIPE_VERTICAL_HALF_HEIGHT/2;
			auto offset = --loopCount * PIPE_VERTICAL_HALF_HEIGHT;

			// Draw half bottom
			CSprites::GetInstance()->Get(ID_PIPE_VETICAL_HALF_BOTTOM)->Draw(x + width / 2, top + offset);
		}
	}

	// Draw horizontal pipe
	else {
		// Draw base pipe
		CSprites::GetInstance()->Get(ID_PIPE_HORIZONTAL_FULL)->Draw(l + PIPE_BASE_HEIGHT / 2, y);

		while (loopCount > 0) {
			// Calculate dynamic part width and offset
			auto left = l + PIPE_BASE_WIDTH + PIPE_HORIZONTAL_HALF_WIDTH/2;
			auto offset = --loopCount * PIPE_HORIZONTAL_HALF_WIDTH;

			// Draw half bottom
			CSprites::GetInstance()->Get(ID_PIPE_HORIZONTAL_HALF_RIGHT)->Draw(left + offset, y);
		}
	}

	// Render bounding box from top left
	RenderBoundingBox(x + width/2, y + height/2);
}

void CPipe::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = l + width;
	b = t + height;
}

void CPipe::SetState(int state) {}