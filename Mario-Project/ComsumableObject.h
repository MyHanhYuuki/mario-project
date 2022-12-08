#pragma once
#include "GameObject.h"
class CComsumableObject : public CGameObject
{
protected:
	CGameObject* container;
public:
	CComsumableObject(float x, float y, CGameObject* container) : CGameObject(x, y)
	{
		this->container = container;
	}
};

