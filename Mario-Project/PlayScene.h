#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "TileMap.h"
//#include "Koopas.h"

#define DEFAULT_ID_HIDDEN_ZONE_BASE_PLATFORM 720
#define BASE_PLATFORM_HEIGHT 16

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;

	CTileLayer* background;
	CTileLayer* forgeground;

	// Chứa những cặp (id, game object) của những game object để liên kết với những game object khác,
	// làm vậy để có thể kích hoạt các sự kiện, ví dụ như khi Mario va chạm ? brick từ bên dưới, thì lát nữa kích hoạt sự kiện Mushroom trồi lên
	// VD: một cặp (mushroom_id, mushroom) dùng để liên kết với một ? brick
	unordered_map<int, LPGAMEOBJECT> itemsInside;

	float base_platform_pos_y;
	float old_base_platform_pos_y;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_TILEMAP(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadTilemap(string tilemapFile);
	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	void MakeCameraFollowMario();
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void SetBasePlatformPosY(float pos_y)
	{
		this->old_base_platform_pos_y = base_platform_pos_y;
		this->base_platform_pos_y = pos_y;
	}
};

typedef CPlayScene* LPPLAYSCENE;

