#pragma once
#include "const.h"
#include "tinyxml.h"
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Tile.h"

#define DEFAULT_ID_HIDDEN_ZONE_BASE_PLATFORM 720
#define BASE_PLATFORM_HEIGHT 16

struct TileSet
{
	int firstgid; // Có thể dùng để đại diện tileset (ID tileset)
	int width;
	int height;
	int margin;
	int spacing;
	int tileCount;
	int columns;
	std::string textureID;
};

class CPlayScene: public CScene
{
	int worldWidth;
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;
	vector<LPTILE> tiles;

	// Chứa những cặp (id, game object) của những game object để liên kết với những game object khác,
	// làm vậy để có thể kích hoạt các sự kiện, ví dụ như khi Mario va chạm ? brick từ bên dưới, thì lát nữa kích hoạt sự kiện Mushroom trồi lên
	// VD: một cặp (mushroom_id, mushroom) dùng để liên kết với một ? brick
	unordered_map<int, LPGAMEOBJECT> itemsInside;

	float base_platform_pos_y;
	float old_base_platform_pos_y;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadMap(string tilemapFile);
	void LoadAssets(LPCWSTR assetFile);
	TileSet* LoadTileSet(TiXmlElement* root);
	void ParseTile(TiXmlElement* layerElement);
public: 
	void Reload();
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	int GetPlayerLevel() { return ((CMario*)player)->GetLevel(); }

	void Clear();
	bool AddGameObjectBefore(LPGAMEOBJECT baseObj, LPGAMEOBJECT newGameObj);
	void PurgeDeletedObjects();

	void MakeCameraFollowMario();
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void SetBasePlatformPosY(float pos_y)
	{
		this->old_base_platform_pos_y = base_platform_pos_y;
		this->base_platform_pos_y = pos_y;
	}

	void FireGainPointEvent(string eventName, float x, float y, int point=100);
};

typedef CPlayScene* LPPLAYSCENE;