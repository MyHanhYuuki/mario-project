#pragma once
#include "Game.h"
#include "Sprites.h"

#define DEFAULT_TILE_WIDTH 16
#define DEFAULT_TILE_HEIGHT 16

#define SPRITE_ID_START_TILEMAP 200000


class CTile
{
private:
	float x, y;
	int width, height;
	int sprite_id;
public:
	CTile()
	{
		this->x = 0;
		this->y = 0;
		this->width = DEFAULT_TILE_WIDTH;
		this->height = DEFAULT_TILE_HEIGHT;
		this->sprite_id = -1;
	}
	CTile(float x, float y, int width, int height, int id)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->sprite_id = id;
	}

	void Render();
};
typedef CTile* LPTILE;

