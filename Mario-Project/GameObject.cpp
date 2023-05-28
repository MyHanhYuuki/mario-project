#include <algorithm>

#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

void CGameObject::RenderBoundingBox(float left, float top)
{
	float l,t,r,b; 
	GetBoundingBox(l, t, r, b);

	// Set draw location on texture
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	// Adjust start position with camera position
	CGame::GetInstance()->Draw(left - cx, top - cy, bbox, &rect, BBOX_ALPHA);
}

void CGameObject::RenderBoundingBox()
{
	RenderBoundingBox(x, y);
}

CGameObject::~CGameObject()
{

}