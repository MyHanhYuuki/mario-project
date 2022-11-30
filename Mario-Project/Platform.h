#pragma once

#include "GameObject.h"

#define FIRST_BASE_PLATFORM 0
#define HIDEEN_ZONE_BASE_PLATFORM 1

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected: 
	int length;				// Unit: cell 
	bool base_platform;
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public: 
	CPlatform(float x, float y, bool is_base,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->base_platform = is_base;
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;