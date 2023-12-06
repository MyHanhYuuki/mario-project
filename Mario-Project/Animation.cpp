#include "Animation.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, D3DXMATRIX* matCustomTranformation)
{
	GetCurrentAnimationFrame()->GetSprite()->Draw(x, y, matCustomTranformation);
}

/** Lấy vị trí khung hình hiện tại */
int CAnimation::CalculateCurrentFrameIndex()
{
	ULONGLONG now = GetTickCount64();
	if (currentFrameIndex == -1)
	{
		currentFrameIndex = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrameIndex]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrameIndex++;
			lastFrameTime = now;
			if (currentFrameIndex == frames.size()) currentFrameIndex = 0;
		}

	}

	return currentFrameIndex;
}

/* Lấy khung hình hiện tại*/
LPANIMATION_FRAME CAnimation::GetCurrentAnimationFrame()
{
	int curentFrame = CalculateCurrentFrameIndex();
	return frames[currentFrameIndex];
}