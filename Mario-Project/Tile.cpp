#include "Tile.h"

void CTile::Render()
{
	CSprites::GetInstance()->Get(sprite_id)->Draw(x*width + width/2, y*height + height/2);
}
