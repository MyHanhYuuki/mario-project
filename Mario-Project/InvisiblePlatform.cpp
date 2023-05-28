#include "InvisiblePlatform.h"

void CInvisiblePlatform::Update(DWORD dt)
{

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
	RenderBoundingBox(x + width/2, y + height/2);
}