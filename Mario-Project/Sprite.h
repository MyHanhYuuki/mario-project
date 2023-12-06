#pragma once

#include "Texture.h"
#include "Game.h"

class CSprite
{
private:
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;
	int width;
	int height;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	// Getters
	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }

	void Draw(float x, float y, D3DXMATRIX* matCustomTranformation = NULL);
};

typedef CSprite* LPSPRITE;