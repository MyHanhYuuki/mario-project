#include "InvisiblePlatform.h"

void CInvisiblePlatform::Update(DWORD dt)
{
	/*DebugOutTitle(L"Invis: %0.2f, %0.2f", x, y);*/
}

void CInvisiblePlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + width;
	b = t + height;
}


void CInvisiblePlatform::Render()
{
	RenderBoundingBox();
}

void CInvisiblePlatform::RenderBoundingBox()
{
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x + this->width/2 - cx, y + this->height/2 - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}
