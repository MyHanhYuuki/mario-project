#pragma once
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
private:
	int scene_id;	// target scene to switch to 

	float width;
	float height; 
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void RenderBoundingBox(void);

	int GetSceneId() { return scene_id;  }
	int IsBlocking() { return 0; }
};