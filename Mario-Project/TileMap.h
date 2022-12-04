#pragma once
#include "GameObject.h"
#include "TileLayer.h"
#include "Utils.h"

#define ID_FIRST_WORLD_MAP_1_1 70

struct TileSet;

class CMap
{
private:
	int firstSpriteId;
	vector<LPTILELAYER> layers;
	string filePath;

	CTileLayer* background;
	CTileLayer* forgeground;

	vector<LPGAMEOBJECT> objects;

	void Load();
	TileSet* LoadTileSet(TiXmlElement* root);
	CTileLayer* LoadLayer(TiXmlElement* layerElement);
	void registerTilesAndSprites(TileSet* root, CTileLayer* layer);
public:
	CMap(string path);

	vector<LPGAMEOBJECT> getMapObject() { return this->objects; }
	void Render(vector<LPGAMEOBJECT> objects);
};
typedef CMap LPTILEMAP;

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