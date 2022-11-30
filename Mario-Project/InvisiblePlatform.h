#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "debug.h"

class CInvisiblePlatform : public CGameObject
{
private:
	int width, height;
public:
	CInvisiblePlatform(float x, float y, int type, int width, int height): CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
	}

	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

