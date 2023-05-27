#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "debug.h"

class CInvisiblePlatform : public CGameObject
{
private:
	int width, height;
public:
	CInvisiblePlatform(float x, float y, int type, int width, int height);
	CInvisiblePlatform(string name, float x, float y, int type, int width, int height);

	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsBlocking() { return blockDirection != BLOCK_NONE; }
};

