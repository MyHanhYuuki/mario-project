#include "InvisiblePlatform.h"

CInvisiblePlatform::CInvisiblePlatform(float x, float y, int type, int width, int height) : CGameObject(x, y)
{
	this->width = width;
	this->height = height;
	this->blockDirection = BLOCK_ALL;
}

CInvisiblePlatform::CInvisiblePlatform(string name, float x, float y, int type, int width, int height) : CInvisiblePlatform(x, y, type, width, height)
{
	this->name = name;

	// Extract direction from name
	if ((int)name.rfind("Top") >= 0) {
		SetBlockDirection(BLOCK_TOP);
	}
}

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