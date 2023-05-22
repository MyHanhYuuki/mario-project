#include "LifeMushRoom.h"

CLifeMushroom::CLifeMushroom(float x, float y) : CMushroom(x, y) {

}

void CLifeMushroom::Render()
{
    CSprites::GetInstance()->Get(ID_SPRITE_LIFE_MUSROOM)->Draw(x, y);

	RenderBoundingBox();
}
