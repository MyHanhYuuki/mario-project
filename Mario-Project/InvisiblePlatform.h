#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "debug.h"

class CInvisiblePlatform : public CGameObject
{
public:
	CInvisiblePlatform(float x, float y, int width, int height) : CGameObject(x, y, width, height) {}
	CInvisiblePlatform(string name, float x, float y, int width, int height) : CInvisiblePlatform(x, y, width, height)
	{
		this->name = name;

		// Extract direction from name
		if ((int)name.rfind("Top") >= 0) {
			this->blockDirection = BLOCK_TOP;
		}
	}
	
	int IsBlocking() { return blockDirection != BLOCK_NONE; }

	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
