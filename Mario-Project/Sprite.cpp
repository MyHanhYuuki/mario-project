#include "Sprite.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
	this->width = abs(right - left);
	this->height = abs(bottom - top);

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / texWidth;
	sprite.TexCoord.y = this->top / texHeight;

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / texWidth;
	sprite.TexSize.y = spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

void CSprite::Draw(float x, float y, D3DXMATRIX* matCustomTranformation)
{
	auto g = CGame::GetInstance();
	auto correctedDrawLocation = g->GetCorrectedDrawLocation(x, y);
	
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, correctedDrawLocation->x, correctedDrawLocation->y, 0.1f);

	// Apply custom tranformation
	this->sprite.matWorld = (this->matScaling * matTranslation);
	if (matCustomTranformation != NULL) {
		D3DXMatrixMultiply(&this->sprite.matWorld, &this->sprite.matWorld, matCustomTranformation);
	}

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}