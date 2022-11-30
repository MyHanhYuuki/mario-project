#pragma once
#include "TileLayer.h"
#include "Utils.h"

#define ID_FIRST_WORLD_MAP_1_1 70

class CTileMap
{
private:
	int sprite_id_start;
	vector<LPTILELAYER> layers;
	string filePath;
public:
	CTileMap();
	CTileMap(string path);

	vector<vector<int>> GetLayerInfo(TiXmlElement* layer_child);
	void GetSpriteInfo(TiXmlElement* root, vector<vector<int>> grid);
	void MakeLayers();

	LPTILELAYER GetBackground() { return this->layers[0]; }
	LPTILELAYER GetForgeground() { return this->layers[1]; }

	void RenderBackground();
	void RenderForgeground();
};
typedef CTileMap LPTILEMAP;


