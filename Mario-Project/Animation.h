#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
private:
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrameIndex;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrameIndex = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, D3DXMATRIX* matCustomTranformation=NULL);
	int CalculateCurrentFrameIndex();
	LPANIMATION_FRAME GetCurrentAnimationFrame();
};

typedef CAnimation* LPANIMATION;